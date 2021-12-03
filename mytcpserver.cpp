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

        float x, y;
        in>>x>>y;
        int j;
        in>>j;
        vector<Barrier> b;
        b.reserve(j);
        for(int i=0; i<j; i++)
        {

            float x1, x2, x3,x4;
            in>>x1>>x2>>x3>>x4;
            Barrier b1(x1,x2,x3,x4);
            b.push_back(b1);

        }

    GameMap g1(Point(x,y),b);
    g1.doo();
    QByteArray arr;
    QDataStream d(&arr, QIODevice::WriteOnly);

    for(int i=0; i<g1.short_path.size();i++)
        d<<g1.short_path[i].x<<g1.short_path[i].y;

    socket->write(arr);
}
