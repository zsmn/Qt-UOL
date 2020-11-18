#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <QHostAddress>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    void sendMsg(std::string msg);

public slots:
    void onReadyRead();
    void onConnected();

private:
    QTcpSocket *_socket;
};

#endif // CLIENT_H
