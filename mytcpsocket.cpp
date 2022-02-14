#include "mytcpsocket.h"
#include<QDataStream>
#include"myexception.h"
#include <QTimer>
#include<QTime>
#include <QCoreApplication>

template<>
void MyLog::printLogToFile( GameMap *o)
{
    o->printToFile(*ofs);
}

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
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

void MyTcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error: " << socketError;
    qDebug()<<"Reconnect!";
    Logger->printLogToFile("Reconnect to host");
    delay(reconnect_time);
    socket->connectToHost(IP, PORT);
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
    qDebug() << "Reconnect!";
    Logger->printLogToFile("Reconnect to host");
    delay(reconnect_time);
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    //Logger->printLogToFile(byte);
    //qDebug() << bytes << " bytes written...";
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
            float width_auto;   //ширина авто
            float height_auto;  //высота авто
            float x, y;         //точка маршрута
            int j;              //количество препятствий
            //int width;
            //int height;



            in>>width_coord>>height_coord>>step>>start;
            in>>width_auto>>height_auto;
            in>>x>>y;
            in>>j;

            //width=width_coord/step;
            //height=height_coord/step;

            /*int x1=(start + int(x/step)) % width;
            int y1=(start - int(y / step) * width) / width;

            float w1=(width)-11%width;
            float w2=(w1)-(width-1);
            float h1=-(height-1)-int(11/width);
            float h2=abs(h1-(height-1));

            float x2=(start%width+int(x/step))*step;
            float y2=(start/width+int(y/step))*step;*/
            //int p=x1+y1;
            //если ввод некорректен
            if(width_coord <=0||height_coord<=0||step<=0||width_auto<0||height_auto<0||j<0||start<0
            ||start>(width_coord/step*height_coord/step))

            {
                throw MyException("Data package error", DataPackageError);
                Logger->printLogToFile("Data package error");
            }
            else
            {
                GameMap g1(width_coord,height_coord,step,start, width_auto, height_auto, Point(x,y));
                float x1, x2, x3=0,x4=0;
                int i=0;
                while (x3>=0 && x4>=0 && i!=j)
                {
                    in>>x1>>x2>>x3>>x4;
                    auto *bar=new BQuadrAngle(x1,x2,x3,x4);
                    g1.barriers.push_back(bar);
                    //qDebug()<<(bar)->hasPoint(g1.goal_point);
                    i++;
                }
                if(i!=j||x3<0||x4<0){
                    throw MyException("Data package error", DataPackageError);
                    Logger->printLogToFile("Data package error");
                }
                Logger->printLogToFile("Data recieve");
                g1.doo(DEBUG_OUTPUT);
                Logger->printLogToFile(&g1);

                //socket->flush();
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<(int)g1.short_path.size();
                for(unsigned int i=0; i<g1.short_path.size();i++)
                    d<<g1.short_path[i].x<<g1.short_path[i].y;
                //delay();
                socket->write(arr);
                socket->flush();
                Logger->printLogToFile("Path send\n");
            }
        }
        socket->readAll();
    }
    catch (MyException& Ex)
    {
        socket->readAll();
        QProcess::execute("clear");
        //socket->flush();
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
