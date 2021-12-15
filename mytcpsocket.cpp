#include "mytcpsocket.h"
#include<QDataStream>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
}

void MyTcpSocket::doConnect()
{
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call

    //"79.164.82.177", 15555
    socket->connectToHost("79.164.82.177", 15555);

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
    int a=0x44;
    int b=0x46;
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
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyRead()
{
    QProcess::execute("clear");
    QObject* object=QObject::sender();
    if(!object)
        return;

    QDataStream in(socket);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);

    unsigned char a,b1;
    in>>a>>b1;
    //cout<<a<<endl<<b1<<endl;

    if(a==0x44 && b1==0x47)
    {
        int ErrorCode=0;    //код ошибки
        float width_coord;  //ширина поля в координатах
        float height_coord; //высота поля в координатах
        float step;         //шаг сетки
        int start;          //начальное положение авто (номер вершины графа)
        float width_auto;   //ширина авто
        float height_auto;  //высота авто
        float x, y;         //точка маршрута
        int j;              //количество препятствий
        vector<Barrier*> b; //препятствия

        in>>width_coord>>height_coord>>step>>start;
        in>>width_auto>>height_auto;
        in>>x>>y;
        in>>j;
        //cout<<width_coord<<endl<<height_coord<<endl<<step<<endl<<start<<endl<<width_auto<<endl<<height_auto<<endl<<x<<endl<<endl<<y<<endl<<endl;

        if(width_coord <=0||height_coord<=0||step<=0||width_auto<0||height_auto<0||j<0||start<0
                ||start>(width_coord/step*height_coord/step))
            ErrorCode=-3;
        else
        {
            b.reserve(j);
            for(int i=0; i<j; i++)
            {
                float x1, x2, x3,x4;
                in>>x1>>x2>>x3>>x4;
                b.push_back(new BQuadrAngle(x1,x2,x3,x4));
                //cout<<x1<<endl<<x2<<endl<<x3<<endl<<x4<<endl<<endl;
            }
            GameMap g1(width_coord,height_coord,step,start, width_auto, height_auto, Point(x,y),b);
            ErrorCode=g1.doo();
            if (ErrorCode==0)
            {
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                //d<<int(0);
                d<<(int)g1.short_path.size();
                for(int i=0; i<g1.short_path.size();i++)
                    d<<g1.short_path[i].x<<g1.short_path[i].y;

                socket->write(arr);
                socket->flush();
                return;
            }
        }
        QByteArray error;
        QDataStream d(&error, QIODevice::WriteOnly);
        d.setFloatingPointPrecision(QDataStream::SinglePrecision);
        d.setByteOrder(QDataStream::LittleEndian);
        d<<(unsigned char)0x44<<(unsigned char)0x48;
        d<<ErrorCode;
        cout<<"Error "<<ErrorCode;
        socket->write(error);
        socket->flush();
    }
}
