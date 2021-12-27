#include "mytcpsocket.h"
#include<QDataStream>
#include<myexception.h>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
}

void MyTcpSocket::doConnect(const QString& IP, const int PORT, const int DEBUG_OUTPUT)
{
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    this->DEBUG_OUTPUT=DEBUG_OUTPUT;
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call

    //"79.164.82.177", 15555
    socket->connectToHost(IP, PORT);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    d<<(unsigned char)0x44<<(unsigned char)0x46;
    socket->write(qb);
    socket->flush();
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
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

            in>>width_coord>>height_coord>>step>>start;
            in>>width_auto>>height_auto;
            in>>x>>y;
            in>>j;

            //если ввод некорректен
            if(width_coord <=0||height_coord<=0||step<=0||width_auto<0||height_auto<0||j<0||start<0
            ||start>(width_coord/step*height_coord/step)){
                throw MyException("Data package error", DataPackageError);}
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
                    i++;
                }
                if(i!=j||x3<0||x4<0){
                    throw MyException("Data package error", DataPackageError);}
                g1.doo(DEBUG_OUTPUT);
                //socket->flush();
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<(int)g1.short_path.size();
                for(unsigned int i=0; i<g1.short_path.size();i++)
                    d<<g1.short_path[i].x<<g1.short_path[i].y;
                socket->write(arr);
                socket->flush();
            }
        }
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
}
