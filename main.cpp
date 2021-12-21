#include <QCoreApplication>
#include <QFile>
//#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "settings.h"
#include "myexception.h"

int main(int argc, char *argv[])
{
    try
    {
        QCoreApplication a(argc, argv);
        //MyTcpServer server;
        QFile f("config.cfg");
        if(!f.open(QIODevice::ReadOnly))
            throw MyException("config.cfg not found", ConfigNotFound);
        Settings::setDefaults(f.readAll());

        int DEBUG_OUTPUT=Settings::get(Settings::DEBUG_OUTPUT).toInt();
        f.close();
        MyTcpSocket s;
        s.doConnect(Settings::get(Settings::IP_SERVER).toString(),Settings::get(Settings::PORT).toInt(),DEBUG_OUTPUT);

        return a.exec();
    }  catch (MyException& Ex)
    {
        qDebug()<<Ex.what();
        qDebug()<<"Error "<<Ex.GetErrorCode();
    }
}
