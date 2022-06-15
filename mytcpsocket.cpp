#include "mytcpsocket.h"
#include<QDataStream>
#include"myexception.h"
#include <QTimer>
#include<QTime>
#include <QCoreApplication>

void MyLog::printLogToFile(const GameMap& map)
{
    GameMapPrinter::printToFile(map,*ofs);
}

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
    isSmoothing=false;
    isConnected=false;
}

void delay(const int RECONNECT_TIME)
{
    QTime dieTime= QTime::currentTime().addMSecs(RECONNECT_TIME);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MyTcpSocket::doConnect(const QString& IP, const int PORT, const int DEBUG_OUTPUT, const bool PRINT_LOG, const int RECONNECT_TIME)
{
    reconnect_time=RECONNECT_TIME;
    this->IP=IP;
    this->PORT=PORT;
    Logger=new MyLog(PRINT_LOG, "log");
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    this->DEBUG_OUTPUT=DEBUG_OUTPUT;
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError )),this,SLOT(Error(QAbstractSocket::SocketError)));
    qDebug() << "connecting...";
    Logger->printLogToFile("Connect to host");
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::reconnect()
{
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error: " << socketError;
    qDebug()<<"Reconnect!";
    Logger->printLogToFile("Reconnect to host");
    QTimer::singleShot(reconnect_time, this, SLOT(reconnect()));
}

void MyTcpSocket::connected()
{
    qDebug()<<"connected";
    Logger->printLogToFile("connected\n");
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    d<<(unsigned char)0x79<<(unsigned char)0x92;
    socket->write(qb);
    socket->flush();
    Logger->printLogToFile("Send 0x79 0x92 to host\n");
}

void MyTcpSocket::disconnected()
{
    Logger->printLogToFile("disconnected");
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{

}

void MyTcpSocket::readyRead()
{
    try
    {
        //QProcess::execute("clear");
        QObject* object=QObject::sender();
        if(!object)
            return;

        QDataStream in(socket);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        in.setByteOrder(QDataStream::LittleEndian);

        unsigned char b1,b2;
        in>>b1>>b2;

        if(b1==0x44 && b2==0x47 && isConnected)
        {
            Logger->printLogToFile("Reciev 0x44 0x47 from host\n");

            float width_coord;  //ширина поля в координатах
            float height_coord; //высота поля в координатах
            float step;         //шаг сетки
            int center;         //начальное положение авто (номер вершины графа)
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float speed;
            Position goal;
            int j;              //количество препятствий

            in>>width_coord>>height_coord>>step>>center;
            in>>goal.x>>goal.y>>goal.z;
            in>>j;

            qDebug()<<"Map:";
            qDebug()<<width_coord<<height_coord<<step<<center;
            qDebug()<<"Goal:";
            qDebug()<<goal.x<<goal.y<<goal.z;
            qDebug()<<"n = "<<j;
            in>>m00>>m01>>m02>>m03>>m10>>m11>>m12>>m13>>m20>>m21>>m22>>m23>>speed;
            qDebug()<<"Car:";
            qDebug()<<m00<<m01<<m02<<m03;
            qDebug()<<m10<<m11<<m12<<m13;
            qDebug()<<m20<<m21<<m22<<m23;
            qDebug()<<speed;

            //GameMap g1(width_coord,height_coord,step,center,
            //            m00, m01, m02, m03,
            //            m10, m11, m12, m13,
            //            m20, m21, m22, m23, speed, Ogre::Vector3(goal.x,goal.y,goal.z), isSmoothing);
                for(int i=1; i<j; i++)
                {
                    in>>m00>>m01>>m02>>m03>>m10>>m11>>m12>>m13>>m20>>m21>>m22>>m23;
                    qDebug()<<"cube "<<i;
                    qDebug()<<m00<<m01<<m02<<m03;
                    qDebug()<<m10<<m11<<m12<<m13;
                    qDebug()<<m20<<m21<<m22<<m23;
                    //BQuadrAngle bar(m00, m01, m02, m03,
                    //                m10, m11, m12, m13,
                    //                m20, m21, m22, m23);
                    //g1.barriers.push_back(&bar);
                }
                Logger->printLogToFile("Data recieve");
                //g1.doo(DEBUG_OUTPUT);
                //Logger->printLogToFile(g1);
                delay(200);
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<0;
                //d<<(int)g1.short_path.size();
                //for(auto it=g1.short_path.begin(); it!=g1.short_path.end();++it)
                //    d<<(*it).x<<(*it).y;
                socket->write(arr);
                socket->flush();
                Logger->printLogToFile("Path send\n");
        }
        //else if(b1==0x44 && b2==0x48)
        //{
            //Logger->printLogToFile("Reciev 0x44 0x48 from host\n");

                //in>>isSmoothing;
            //qDebug()<<"Change Mode "<<isSmoothing;
        //}
        else if(b1==0x79 && b2==0x93)
        {
            Logger->printLogToFile("Reciev 0x79 0x93 from host\n");
            isConnected=true;
            QByteArray arr;
            QDataStream d(&arr, QIODevice::WriteOnly);
            d.setFloatingPointPrecision(QDataStream::SinglePrecision);
            d.setByteOrder(QDataStream::LittleEndian);
            Logger->printLogToFile("Send 0x79 0x94 to host\n");
            d<<(unsigned char)0x79<<(unsigned char)0x94;
            socket->write(arr);
            socket->flush();
        }
        else
            Logger->printLogToFile("Unknow command\n");

        socket->readAll();
    }
    catch (MyException& Ex)
    {
        socket->readAll();
        QProcess::execute("clear");
        qDebug()<<Ex.what();
        qDebug()<<"Error "<<Ex.GetErrorCode();
        QByteArray error;
        QDataStream d(&error, QIODevice::WriteOnly);
        d.setFloatingPointPrecision(QDataStream::SinglePrecision);
        d.setByteOrder(QDataStream::LittleEndian);
        d<<(unsigned char)0x44<<(unsigned char)0x48;
        d<<Ex.GetErrorCode();
        socket->write(error);
        socket->flush();
    }
}

MyTcpSocket::~MyTcpSocket()
{
    delete socket;
    delete Logger;
}
