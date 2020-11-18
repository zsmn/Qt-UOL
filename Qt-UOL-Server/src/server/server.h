#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <QSignalMapper>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

public slots:
    void newConnection();
    void sendWelcomeMessage();
    void broadcast(QString);
    void rcvMsg(QTcpSocket *socket);
    void sendMsg(QString msg, QTcpSocket *socket);

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QSignalMapper *signalmapper;

    QVector<QTcpSocket *> _sockets;
};

#endif // SERVER_H
