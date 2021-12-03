#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QTcpServer>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();
    void readyRead();
private:
    void sendToClient(QTcpSocket* socket, QDataStream& data);
    QTcpServer *server;
};

#endif // MYTCPSERVER_H
