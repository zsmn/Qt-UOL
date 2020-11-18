#include "client.h"

Client::Client(QString ip, int porta, QString name)
{
    _socket = new QTcpSocket(this);
    _name = name;
    _socket->connectToHost(QHostAddress(ip), porta);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
}

void Client::onConnected(){
    _socket->write(QByteArray(QString(":config:"+_name).toStdString().c_str()));
    _socket->waitForBytesWritten(30);
}

void Client::onReadyRead(){
    QString data(_socket->readAll());
    QStringList listWords = data.split(":");
    if(listWords.value(1)=="listaNomes"){
        listWords.removeAt(0);
        listWords.removeAt(0);
        listNames = new QStringList("broadcast");
        for(auto it = listWords.begin(); it != listWords.end(); ++it)
            listNames->append(listWords); //contem a lista dos nomes que devem aparecer no campo destino

    }

    std::cout << data.toStdString() << std::endl;
}

void Client::sendMsg(std::string msg){
    _socket->write(QByteArray(msg.c_str()));
}
