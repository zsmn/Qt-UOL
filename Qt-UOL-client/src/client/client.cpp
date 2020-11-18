#include "client.h"

Client::Client()
{
    _socket = new QTcpSocket(this);

    _socket->connectToHost(QHostAddress("127.0.0.1"), 9999);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
}

void Client::onConnected(){
    _socket->write(QByteArray("conectei e mandei"));
}

void Client::onReadyRead(){
    QByteArray data = _socket->readAll();
    std::cout << data.toStdString() << std::endl;
}

void Client::sendMsg(std::string msg){
    _socket->write(QByteArray(msg.c_str()));
}
