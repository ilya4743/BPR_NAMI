#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include<QProcess>
#include "gamemap.h"
#include "mylog.h"

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

    void doConnect(const QString& IP, const int PORT, const int DEBUG_OUTPUT);
    ~MyTcpSocket();
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    int DEBUG_OUTPUT;
    MyLog *Logger;
};

#endif // MYTCPSOCKET_H
