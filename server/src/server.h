#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <iostream>
#include <include/communication.pb.h>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(quint16 serverPort);
    ~Server();

private:
    // Server
    QTcpServer *_server;
    quint16 _serverPort;

    // Clients
    QSignalMapper *_signalMapper;
    QMap<QTcpSocket*, std::string> _clients;

    // Utils
    void sendMessage(QString message, QTcpSocket *socket, QString sender, bool isPrivate);
    void sendMessage(ServerMessage message, QTcpSocket *socket);
    void broadcastMessage(QString message, QString sender);
    int getUsersWithThatName(QString name);
    QTcpSocket* requestReceiver(QString receiverName);

public slots:
    void processUser();
    void broadcastConnectedClients();
    void processMsg(QObject *);

};

#endif // SERVER_H
