#include "client.h"

Client::Client(QString name)
{
    _socket = new QTcpSocket(this);
    _name = name;
    _socket->connectToHost(QHostAddress("127.0.0.1"), 9999);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
}

void Client::onConnected(){
    _socket->write(QByteArray(QString(":config:"+_name).toStdString().c_str()));
    _socket->waitForBytesWritten(30);
}

void Client::onReadyRead(){
    QByteArray data = _socket->readAll();
    std::cout << data.toStdString() << std::endl;
}

void Client::sendMsg(std::string msg){
    _socket->write(QByteArray(msg.c_str()));
}
