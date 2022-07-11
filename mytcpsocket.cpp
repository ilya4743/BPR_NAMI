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
            Position goal;
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
            GameMap g1(width_coord,height_coord,step,center,Car(*itCar,speed), Ogre::Vector3(goal.x,goal.y,goal.z), isSmoothing);

            qDebug()<<"id"<<itCar.key()<<"\tcar"<<": ";
            qDebug()<<"position\t"<<g1.car.position.x<<'\t'<<g1.car.position.y<<'\t'<<g1.car.position.z;
            qDebug()<<"rotation\t"<<g1.car.rotation.w<<'\t'<<g1.car.rotation.x<<'\t'<<g1.car.rotation.y<<'\t'<<g1.car.rotation.z;
            qDebug()<<"scale\t"<<g1.car.scale.x<<'\t'<<g1.car.scale.y<<'\t'<<g1.car.scale.z;
            qDebug()<<"speed\t"<<speed;

            for(auto itGameObj=mapGameObj.begin();itGameObj!=mapGameObj.end();++itGameObj)
            {
                auto *bar=new BQuadrAngle(itGameObj.value());

                qDebug()<<"id"<<itGameObj.key()<<"\tcube:";
                qDebug()<<"p1\t"<<(bar)->p1.x<<'\t'<<(bar)->p1.y<<'\t'<<(bar)->p1.z;
                qDebug()<<"p2\t"<<(bar)->p2.x<<'\t'<<(bar)->p2.y<<'\t'<<(bar)->p2.z;
                qDebug()<<"p7\t"<<(bar)->p7.x<<'\t'<<(bar)->p7.y<<'\t'<<(bar)->p7.z;
                qDebug()<<"p8\t"<<(bar)->p8.x<<'\t'<<(bar)->p1.y<<'\t'<<(bar)->p8.z;
                qDebug()<<"globpos\t"<<(bar)->position.x<<'\t'<<(bar)->position.y<<'\t'<<(bar)->position.z;

                (*bar).mat4=g1.car.matrix4.inverse()*((*bar).mat4);
                (*bar).p1=g1.car.matrix4.inverse()*(*bar).p1;
                (*bar).p2=g1.car.matrix4.inverse()*(*bar).p2;
                (*bar).p3=g1.car.matrix4.inverse()*(*bar).p3;
                (*bar).p4=g1.car.matrix4.inverse()*(*bar).p4;
                (*bar).p5=g1.car.matrix4.inverse()*(*bar).p5;
                (*bar).p6=g1.car.matrix4.inverse()*(*bar).p6;
                (*bar).p7=g1.car.matrix4.inverse()*(*bar).p7;
                (*bar).p8=g1.car.matrix4.inverse()*(*bar).p8;
                Ogre::Affine3 af((*bar).mat4);
                af.decomposition((*bar).position,(*bar).scale, (*bar).rotation);

                qDebug()<<"localpos\t"<<(bar)->position.x<<'\t'<<(bar)->position.y<<'\t'<<(bar)->position.z;
                qDebug()<<"rotation\t"<<(bar)->rotation.w<<'\t'<<(bar)->rotation.x<<'\t'<<(bar)->rotation.y<<'\t'<<(bar)->rotation.z;
                qDebug()<<"scale\t"<<(bar)->scale.x<<'\t'<<(bar)->scale.y<<'\t'<<(bar)->scale.z;
                qDebug()<<"p1\t"<<(bar)->p1.x<<'\t'<<(bar)->p1.y<<'\t'<<(bar)->p1.z;
                qDebug()<<"p2\t"<<(bar)->p2.x<<'\t'<<(bar)->p2.y<<'\t'<<(bar)->p2.z;
                qDebug()<<"p7\t"<<(bar)->p7.x<<'\t'<<(bar)->p7.y<<'\t'<<(bar)->p7.z;
                qDebug()<<"p8\t"<<(bar)->p8.x<<'\t'<<(bar)->p1.y<<'\t'<<(bar)->p8.z;

                g1.barriers.push_back(bar);
            }


                g1.doo(DEBUG_OUTPUT);

                QByteArray arr;
                QDataStream d(&arr, QIODevice::WriteOnly);
                d.setFloatingPointPrecision(QDataStream::SinglePrecision);
                d.setByteOrder(QDataStream::LittleEndian);
                d<<(unsigned char)0x44<<(unsigned char)0x48;
                d<<(int)g1.short_path.size();
                for(auto it=g1.short_path.begin(); it!=g1.short_path.end();++it)
                {
                    d<<(*it).x<<(*it).y;
                    //qDebug("%f\t%f;", (*it).x, (*it).y);
                }
                socket->write(arr);
                socket->flush();
                qDebug()<<"path send to host\n";
        }
        //else if(b1==0x44 && b2==0x48)
        //{

                //in>>isSmoothing;
            //qDebug()<<"Change Mode "<<isSmoothing;
        //}
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
    delete Logger;
}
