#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

public slots:
    void newConnection();

private:
    QTcpServer *server;

    QVector<QTcpSocket *> _sockets;
};

#endif // SERVER_H
