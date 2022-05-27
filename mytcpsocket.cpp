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
    d<<(unsigned char)0x44<<(unsigned char)0x46;
    socket->write(qb);
    socket->flush();
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
        QProcess::execute("clear");
        QObject* object=QObject::sender();
        if(!object)
            return;

        QDataStream in(socket);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        in.setByteOrder(QDataStream::LittleEndian);

        unsigned char b1,b2;
        in>>b1>>b2;

        if(b1==0x44 && b2==0x47)
        {
            float width_coord;  //ширина поля в координатах
            float height_coord; //высота поля в координатах
            float step;         //шаг сетки
            int start;          //начальное положение авто (номер вершины графа)
            Car car;
            //float width_auto;   //ширина авто
            //float height_auto;  //высота авто
            //float x, y;         //точка маршрута
            Position goal;
            int j;              //количество препятствий

            in>>width_coord>>height_coord>>step>>start;
            in>>car;
            in>>goal;
            in>>j;

            //если ввод некорректен
            //if(width_coord <=0||height_coord<=0||step<=0||width_auto<0||height_auto<0||j<0||start<0
            //||start>(width_coord/step*height_coord/step))

            //{
            //    throw MyException("Data package error", DataPackageError);
            //    Logger->printLogToFile("Data package error");
            //}
            //else
            {
                GameMap g1(width_coord,height_coord,step,start, car.scale.x, car.scale.y, Point(goal.x,goal.y), isSmoothing);
                int i=0;
                while (in.atEnd())
                {
                    BQuadrAngle bar;
                    in>>bar;
                    g1.barriers.push_back(&bar);
                    i++;
                }
                Logger->printLogToFile("Data recieve");
                g1.doo(DEBUG_OUTPUT);
                Logger->printLogToFile(g1);
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<(int)g1.short_path.size();
                for(auto it=g1.short_path.begin(); it!=g1.short_path.end();++it)
                    d<<(*it).x<<(*it).y;
                socket->write(arr);
                socket->flush();
                Logger->printLogToFile("Path send\n");
            }
        }
        else if(b1==0x44 && b2==0x48)
        {
                in>>isSmoothing;
            qDebug()<<"Change Mode "<<isSmoothing;
        }
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
