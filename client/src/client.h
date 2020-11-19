#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <include/communication.pb.h>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ip, quint16 port, QString name);
    ~Client();
    void sendMsg(QString msg, QString destName = "");

private:
    QTcpSocket *_socket;
    QString _name;

    void sendMessage(ClientMessage msg);

public slots:
    void onReadyRead();
    void onConnected();

signals:
    void upcomingMessage(QString text, QString userName, bool isPrivate);
    void onlineUsers(QList<QString> list);
};

#endif // CLIENT_H
