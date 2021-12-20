#include "mytcpserver.h"
#include<QProcess>
#include "gamemap.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 2626))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyTcpServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

void MyTcpServer::readyRead()
{
    QProcess::execute("clear");
    QObject* object=QObject::sender();
    if(!object)
        return;
    QTcpSocket* socket=(QTcpSocket*)sender();
    QDataStream in(socket);

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
    if(width_coord <=0||height_coord<=0||step<=0||width_auto<0||height_auto<0||j<0||start<0||start>(width_coord/step*height_coord/step))
        ErrorCode=-3;
    else
    {
        b.reserve(j);
        for(int i=0; i<j; i++)
        {
            float x1, x2, x3,x4;
            in>>x1>>x2>>x3>>x4;
            b.push_back(new BQuadrAngle(x1,x2,x3,x4));
        }

        GameMap g1(width_coord,height_coord,step,start, width_auto, height_auto, Point(x,y));
        g1.doo();
        if (ErrorCode==0)
        {
            QByteArray arr;
            QDataStream d(&arr, QIODevice::WriteOnly);
            d<<int(0);
            for(int i=0; i<g1.short_path.size();i++)
                d<<g1.short_path[i].x<<g1.short_path[i].y;

            socket->write(arr);
            return;
        }
    }
    QByteArray error;
    QDataStream d(&error, QIODevice::WriteOnly);
    d<<ErrorCode;
    socket->write(error);
}
