#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));


    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void Server::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();

    // Erasing non-connected sockets
    for(int x = 0; x < _sockets.size(); x++){
        if(_sockets.at(x)->state() == QAbstractSocket::UnconnectedState){
            QTcpSocket *socket = _sockets.at(x);
            _sockets.removeAt(x);
            delete socket;
            x--;
        }
    }

    socket->write(QByteArray(QString("Hello client allies %1\r\n").arg(_sockets.size()).toStdString().c_str()));
    socket->flush();

    socket->waitForBytesWritten(3000);

    _sockets.push_back(socket);
}
