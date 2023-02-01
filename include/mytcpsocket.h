#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QProcess>

#include "gamemap.h"
#include "car.h"
#include "data_unpacker.h"
#include "occupancygrid.h"
#include "placer.h"

class MyTcpSocket : public QObject
{
    Q_OBJECT

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void Error(QAbstractSocket::SocketError socketError);
    void reconnect();

public:
    explicit MyTcpSocket(QObject *parent = 0);
    void doConnect(const QString& IP, const int PORT, const int DEBUG_OUTPUT,const bool PRINT_LOG,const int RECONNECT_TIME);
    void send_msg(std::string && msg);

    ~MyTcpSocket();
private:
    QTcpSocket *socket;
    int DEBUG_OUTPUT;
    int reconnect_time;
    QString IP;
    int PORT;
    bool isSmoothing;
    bool isConnected;
    DataUnpacker unpacker;
    OccupancyGrid grid;
    Placer placer;
};

#endif // MYTCPSOCKET_H
