#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <QHostAddress>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ip,int porta, QString name);
    void sendMsg(std::string msg);

public slots:
    void onReadyRead();
    void onConnected();

private:
    QTcpSocket *_socket;
    QString _name;
    QStringList *listNames;
};

#endif // CLIENT_H
