#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(sendWelcomeMessage()));


    signalmapper = new QSignalMapper();

    if(!server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}




void Server::sendWelcomeMessage()
{


    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), signalmapper, SLOT(map())); // Signal mapper is defined in the class definition and created in the constructor, see QSignalMapper.

    signalmapper->setMapping(socket, socket);

    connect(signalmapper, SIGNAL(mapped(QObject *)), this, SLOT(rcvMsg(QObject *)));

    for(int x = 0; x < _sockets.size(); x++){
        if(_sockets.at(x).first->state() == QAbstractSocket::UnconnectedState){
            QTcpSocket *socket = _sockets.at(x).first;
            _sockets.removeAt(x);
            delete socket;
            x--;
        }
    }


    _sockets.push_back(std::make_pair(socket, ""));
}

void Server::rcvMsg(QObject *socket){
     QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(socket);

    QString msg(clientSocket->readAll());
    clientSocket->peerAddress();
    std::cout<<clientSocket->peerPort()<<std::endl;

    std::cout<<msg.toStdString()<<std::endl;
    QStringList listWords = msg.split(":");
    QString neededWord = listWords.value(1);
    QString msgSnd="";
    std::cout<<neededWord.toStdString()<<std::endl;
    for(int i=2;i<listWords.size();i++){
        msgSnd+=listWords.value(i);
    }
    if(neededWord.contains("broadcast")){ // se vem uma mensagem de broadcast, envia pra todo mundo
        std::cout<<"entrei broad"<<std::endl;
        broadcast(msgSnd);
    }else if(neededWord.contains("config")){ // se vem uma mensagem de config, faz a atribuição do nome ao socket
        std::cout<<"entrei config"<<std::endl;

        QString listaNomes(":listaNomes:");
        for(int x = 0; x < _sockets.size(); x++){
            if(_sockets.at(x).second!="") listaNomes+=_sockets.at(x).second+":";

            if(_sockets.at(x).first->peerPort()==clientSocket->peerPort()){
                _sockets.replace(x, std::make_pair(_sockets.at(x).first, msgSnd));
               listaNomes+=_sockets.at(x).second+":";
            }
        }
        broadcast(listaNomes);

    }else{ // Se vem o nome de uma pessoa conectada, procura qual socket está conectado com aquele nome e envia
        for(int x = 0; x < _sockets.size(); x++){
            if(_sockets.at(x).second==neededWord){
                sendMsg(msg,_sockets.at(x).first);
            }
        }
        std::cout<<"entrei else"<<std::endl;
        clientSocket->waitForBytesWritten(3000);
    }


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
        if(_sockets.at(x).first->state() == QAbstractSocket::UnconnectedState){
            QTcpSocket *socket = _sockets.at(x).first;
            _sockets.removeAt(x);
            delete socket;
            x--;
        }
    }

    for(int x = 0; x < _sockets.size(); x++){
        socket =_sockets.at(x).first;
        sendMsg(msg, socket);
    }

}
