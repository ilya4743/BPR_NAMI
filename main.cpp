#include <QCoreApplication>
#include <QFile>
//#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "settings.h"
#include "myexception.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    try {
        //MyTcpServer server;
        QFile f("config.cfg");
        if(!f.open(QIODevice::ReadOnly))
            throw MyException("Config.cfg not found", ConfigNotFound);
        Settings::setDefaults(f.readAll());

        int p=Settings::get(Settings::DEBUG_OUTPUT).toInt();

        MyTcpSocket s;
        s.doConnect(Settings::get(Settings::IP_SERVER).toString(),Settings::get(Settings::PORT).toInt());
    }  catch (MyException& Ex) {
        qDebug()<<Ex.GetErrorCode();

    }
    return a.exec();
}
