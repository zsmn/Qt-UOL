#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(sendWelcomeMessage()));




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
    socket = server->nextPendingConnection();

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


void Server::sendWelcomeMessage()
{


    socket = server->nextPendingConnection();
//    QString msg(socket->readAll());

//    std::cout<<msg.toStdString()<<std::endl;

    connect(socket, SIGNAL(readyRead()), signalmapper, SLOT(map())); // Signal mapper is defined in the class definition and created in the constructor, see QSignalMapper.

    signalmapper->setMapping(socket, socket);

    connect(signalmapper, SIGNAL(mapped(QObject *)), this, SLOT(rcvMsg(QObject *)));

    for(int x = 0; x < _sockets.size(); x++){
        if(_sockets.at(x)->state() == QAbstractSocket::UnconnectedState){
            QTcpSocket *socket = _sockets.at(x);
            _sockets.removeAt(x);
            delete socket;
            x--;
        }
    }

    socket->write(QByteArray(QString("Hello,").toStdString().c_str()));
    socket->waitForBytesWritten(3000);

    _sockets.push_back(socket);
}

void Server::rcvMsg(QTcpSocket *socket){


    QString msg(socket->readAll());

    QStringList listWords = msg.split(":");
    QString neededWord = listWords.value(0);
    msg="";
    for(int i=1;i<listWords.size();i++){
        msg+=listWords.value(i);
    }
    if(neededWord == "broadcast"){
        broadcast(msg);
    }else{
//        sendMsg(msg,); precisa descobrir pra quem deve enviar pra depois enviar
    }
    printf("entrei");

}

void Server::sendMsg(QString msg, QTcpSocket *socket){
    socket->write(QByteArray(msg.toStdString().c_str()));
    socket->flush();
    socket->waitForBytesWritten(3000);

}

void Server::broadcast(QString msg)
{

    // Erasing non-connected sockets
    for(int x = 0; x < _sockets.size(); x++){
        if(_sockets.at(x)->state() == QAbstractSocket::UnconnectedState){
            QTcpSocket *socket = _sockets.at(x);
            _sockets.removeAt(x);
            delete socket;
            x--;
        }
    }

    for(int x = 0; x < _sockets.size(); x++){
        socket =_sockets.at(x);
        sendMsg(msg, socket);
    }

}
