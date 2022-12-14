#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QProcess>
#include "gamemap.h"
#include "car.h"
#include "DataUnpacker.h"

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

    void doConnect(const QString& IP, const int PORT, const int DEBUG_OUTPUT,const bool PRINT_LOG,const int RECONNECT_TIME);
    ~MyTcpSocket();
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void Error(QAbstractSocket::SocketError socketError);
    void reconnect();
private:
    QTcpSocket *socket;
    int DEBUG_OUTPUT;
    int reconnect_time;
    QString IP;
    int PORT;
    bool isSmoothing;
    bool isConnected;
    DataUnpacker unpacker;

};

#endif // MYTCPSOCKET_H
