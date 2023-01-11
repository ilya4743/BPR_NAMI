#include "mytcpsocket.h"
#include<QDataStream>
#include"myexception.h"
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include "hybridastar.h"
#include "occupancygrid.h"

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
    isSmoothing=false;
    isConnected=false;
    socket = new QTcpSocket(this);
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
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    this->DEBUG_OUTPUT=DEBUG_OUTPUT;
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError )),this,SLOT(Error(QAbstractSocket::SocketError)));
    qDebug() << "connect to "<<IP<<':'<<PORT;
    qDebug() << "connecting...";
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::reconnect()
{
    qDebug()<<"reconnect to "<<IP<<':'<<PORT;
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Connection lost! Error: " << socketError;
    QTimer::singleShot(reconnect_time, this, SLOT(reconnect()));
}

void MyTcpSocket::connected()
{
    qDebug()<<"connected";
    QByteArray qb;
    QDataStream d(&qb, QIODevice::WriteOnly);
    qDebug()<<"send 0x79 0x92 to host";
    d<<(unsigned char)0x79<<(unsigned char)0x92;
    socket->write(qb);
    socket->flush();
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug()<<bytes<<"\tbytes write\n";
}

void MyTcpSocket::send_msg(std::string && msg)
{
    socket->write(msg.c_str(),msg.size());
    socket->flush();
}

void MyTcpSocket::readyRead()
{
    try
    {
        QObject* object=QObject::sender();
        if(!object)
           return;
        string data=socket->readAll().toStdString();
        int s=data.size();
        cout<<s;
        if((unsigned char)data[0]==0x79 && (unsigned char)data[1]==0x93)
        {
            qDebug()<<"Reciev 0x79 0x93 from host";
            qDebug()<<"connection confirmed";
            isConnected=true;
            std::stringstream stream;
            stream<<(unsigned char)0x79<<(unsigned char)0x94;
            send_msg(std::move(stream.str()));
            qDebug()<<"send 0x79 0x94 to host";
        }
        else if(isConnected)
        {
            unpacker.unpack(std::move(data));
            if(unpacker.isCompleted())
            {
                QProcess::execute("clear");

                OccupancyGrid grid(unpacker.width,unpacker.height,unpacker.resolution);
                auto itCar=unpacker.map_mat4_.find(0);
                unpacker.map_mat4_.erase(0);
                                
                Ogre::Matrix4 mat4=(*itCar).second;
                auto qua=(*itCar).second.extractQuaternion();
                Ogre::Vector3 pos1(-unpacker.width/2,mat4.getTrans().y,-unpacker.height/2);
                pos1=mat4*pos1;
                Ogre::Matrix4 mat41(mat4);
                mat41.setTrans(pos1);
                mat4=mat41.inverse()*mat4;

                for(auto itGameObj=unpacker.map_mat4_.begin();itGameObj!=unpacker.map_mat4_.end();++itGameObj)
                {                
                    BQuadrAngle bar(mat41.inverse()*(*itGameObj).second);
                    Placer placer;
                    placer.placeObstacleOnGrid(grid,bar);
                }
                // if(DEBUG_OUTPUT)
                //     for(int i=grid.height-1; i>=0; i--)
                //     {
                //         for (int j=grid.width-1; j>=0; j--)
                //         {
                //             if(grid.data[i*grid.width+j]==100)
                //             cout<<1;
                //             else cout<<0;
                //         }
                //         cout<<endl;
                //     }            
                Car car(mat4,unpacker.speed);

                float x=unpacker.x+car.position.x;
                float y=unpacker.theta+car.position.z; //z
                HybridAstarAlgo hybrid;
                vector<Ogre::Vector3> out =hybrid.searchHybridAStar(car.position.x,car.position.z,1.57,x,y,1.57, grid);           
                qDebug()<<"path:";
                std::stringstream stream;
                stream<<(unsigned char)0x44<<(unsigned char)0x48<<convertToBytes<int>(out.size()*2);
                for(auto it=out.begin(); it!=out.end();++it)
                {
                    (*it).x=(*it).x-car.position.x;
                    (*it).z=(*it).z-car.position.z;
                    *it=qua*(*it);
                    stream<<convertToBytes<float>(-(*it).x)<<convertToBytes<float>((*it).z);
                    //qDebug("%f\t%f;", (*it).x, (*it).z);
                }
                send_msg(std::move(stream.str()));       
                qDebug()<<"path send to host\n";   
            }
        }
        cout<<"wait\n";
    }
    catch (MyException& Ex)
    {
        qDebug()<<Ex.what();
        qDebug()<<"Error "<<Ex.GetErrorCode();
        std::stringstream stream;
        stream<<(unsigned char)0x44<<(unsigned char)0x48<<Ex.GetErrorCode();
        send_msg(std::move(stream.str()));       
        qDebug()<<"send 0x44 0x48 to host";
    }
}

MyTcpSocket::~MyTcpSocket()
{
    delete socket;
}
