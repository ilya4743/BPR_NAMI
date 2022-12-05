#include "mytcpsocket.h"
#include<QDataStream>
#include"myexception.h"
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include "hybridastar.h"
#include "occurancygrid.h"

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

void MyTcpSocket::readyRead()
{
    try
    {
        QProcess::execute("clear");
        QObject* object=QObject::sender();
        if(!object)
            return;
        qDebug()<<socket->size()<<"\tbytes reciev";
        QDataStream in(socket);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
        in.setByteOrder(QDataStream::LittleEndian);

        unsigned char b1,b2;
        in>>b1>>b2;

        if(b1==0x44 && b2==0x47 && isConnected)
        {
            qDebug()<<"reciev 0x44 0x47 from host";
            qDebug()<<"request for pathfinding";
            float width_coord;  //ширина поля в координатах
            float height_coord; //высота поля в координатах
            float step;         //шаг сетки
            int center;         //начальное положение авто (номер вершины графа)
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
            float speed;
            Ogre::Vector3 goal;
            quint64 j, id;              //количество препятствий
            QMap<quint64, Ogre::Matrix4> mapGameObj;
            //(width_coord/step)/2+(height_coord/step-1)*(width_coord/step)
            in>>width_coord>>height_coord>>step>>center;
            if(center<0 || center>((width_coord/step)*(height_coord/step)))
                throw  MyException("DataPackageError", DataPackageError);
            in>>goal.x>>goal.y>>goal.z;
            in>>speed;
            in>>j;

            qDebug()<<"Map:";
            qDebug()<<width_coord<<height_coord<<step<<center;
            qDebug()<<"n = "<<j;
            qDebug()<<"Goal:";
            qDebug()<<goal.x<<goal.y<<goal.z;

            for(int i=0; i<j; i++)
            {
                in>>id;
                in>>m00>>m01>>m02>>m03>>m10>>m11>>m12>>m13>>m20>>m21>>m22>>m23>>m30>>m31>>m32>>m33;

                mapGameObj.insert(id, Ogre::Matrix4(m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33));
            }

            auto itCar=(mapGameObj.find(0));
            mapGameObj.erase(itCar);

            Ogre::Matrix4 mat4=(*itCar);
            auto qua=(*itCar).extractQuaternion();
            Ogre::Vector3 pos1(-width_coord/2,mat4.getTrans().y,-height_coord/2);
            pos1=mat4*pos1;
            Ogre::Matrix4 mat41(mat4);
            mat41.setTrans(pos1);
            mat4=mat41.inverse()*mat4;
            
            GameMap g1(width_coord,height_coord,step,center,Car(mat4,speed), Ogre::Vector3(goal.x,goal.y,goal.z));

            qDebug()<<"id"<<itCar.key()<<"\tcar"<<": ";
            qDebug()<<"position\t"<<g1.car.position.x<<'\t'<<g1.car.position.y<<'\t'<<g1.car.position.z;
            qDebug()<<"rotation\t"<<g1.car.rotation.w<<'\t'<<g1.car.rotation.x<<'\t'<<g1.car.rotation.y<<'\t'<<g1.car.rotation.z;
            qDebug()<<"scale\t"<<g1.car.scale.x<<'\t'<<g1.car.scale.y<<'\t'<<g1.car.scale.z;
            qDebug()<<"speed\t"<<speed;

            OccurancyGrid grid(width_coord,height_coord,step);
            for(auto itGameObj=mapGameObj.begin();itGameObj!=mapGameObj.end();++itGameObj)
            {                
                BQuadrAngle bar(mat41.inverse()*itGameObj.value());
                g1.barriers.push_back(bar);                
                Placer placer;
                placer.placeObstacleOnGrid(grid,bar);
            }

                for(int i=grid.height-1; i>=0; i--)
                {
                    for (int j=grid.width-1; j>=0; j--)
                    {
                        if(grid.data[i*grid.width+j]==255)
                        cout<<1;
                        else cout<<0;
                    }
                    cout<<endl;
                }


                //g1.doo(DEBUG_OUTPUT);
                //HybridAstarAlgo hybrid;
                vector<Ogre::Vector3> out;// =hybrid.searchHybridAStar(g1.car.position.x,g1.car.position.z,1.57,g1.car.position.x,g1.car.position.z+99,1.57,width_coord,height_coord);
                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);              
                qDebug()<<"path:";
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<(int)out.size()*2;
                for(auto it=out.begin(); it!=out.end();++it)
                {
                    (*it).x=(*it).x-200;
                    (*it).z=(*it).z-100;
                    *it=qua.Inverse()*(*it);
                    d<<(*it).x<<(*it).z;
                    //qDebug("%f\t%f;", (*it).x, (*it).z);
                }
                socket->write(arr);
                socket->flush();
                qDebug()<<"path send to host\n";
        }
        else if(b1==0x79 && b2==0x93)
        {
            qDebug()<<"Reciev 0x79 0x93 from host";
            qDebug()<<"connection confirmed";
            isConnected=true;
            QByteArray arr;
            QDataStream d(&arr, QIODevice::WriteOnly);
            d.setFloatingPointPrecision(QDataStream::SinglePrecision);
            d.setByteOrder(QDataStream::LittleEndian);
            qDebug()<<"send 0x79 0x94 to host";
            d<<(unsigned char)0x79<<(unsigned char)0x94;
            socket->write(arr);
            socket->flush();
        }
        else
        {
            qDebug()<<"Unknow command\n";
        }


        socket->readAll();
    }
    catch (MyException& Ex)
    {
        socket->readAll();
        //QProcess::execute("clear");
        qDebug()<<Ex.what();
        qDebug()<<"Error "<<Ex.GetErrorCode();
        QByteArray error;
        QDataStream d(&error, QIODevice::WriteOnly);
        d.setFloatingPointPrecision(QDataStream::SinglePrecision);
        d.setByteOrder(QDataStream::LittleEndian);
        qDebug()<<"send 0x44 0x48 to host";
        qDebug()<<"error "<<Ex.what();
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
