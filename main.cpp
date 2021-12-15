#include <QCoreApplication>
//#include "mytcpserver.h"
#include "mytcpsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MyTcpServer server;

    MyTcpSocket s;
    s.doConnect();

    return a.exec();
}

/*
 * 101
101
1
10150
2
4
0

50
0
0
10
10*/



