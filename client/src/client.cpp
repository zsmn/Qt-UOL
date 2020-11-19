#include "client.h"

Client::Client(QString ip, quint16 port, QString name)
{
    _socket = new QTcpSocket();
    _name = name;

    _socket->connectToHost(QHostAddress(ip), port);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(connected()), this, SLOT(onConnected()));
}

Client::~Client()
{
    _socket->close();
    delete _socket;
}

void Client::onConnected(){
    // Creating packet
    ClientMessage msg;

    // Setting client name and private msg
    msg.set_clientname(_name.toStdString());
    msg.set_clientmessage("");
    msg.set_isprivate(true);

    // Sending
    sendMessage(msg);
}

void Client::sendMessage(ClientMessage msg){
    // Serializing protobuf message to string
    std::string buffer;
    msg.SerializeToString(&buffer);

    // Writing to socket
    if(_socket->write(buffer.c_str(), static_cast<qint64>(buffer.length())) == -1){
        std::cout << "[SERVER] Failed to write to server socket: " << _socket->errorString().toStdString() << std::endl;
    }
}

void Client::sendMsg(QString msg, QString destName){
    // Creating packet
    ClientMessage clientMsg;

    // Setting private, message and receiver name
    clientMsg.set_isprivate((destName != "Broadcast"));
    clientMsg.set_clientmessage(msg.toStdString());
    clientMsg.set_rcvname((destName == "Broadcast" ? "" : destName.toStdString()));

    // Sending
    sendMessage(clientMsg);
}

void Client::onReadyRead(){
    // Reading msg
    ServerMessage message;
    QString buffer(_socket->readAll());

    // Parsing to protobuf msg
    if(message.ParseFromArray(buffer.toStdString().c_str(), buffer.length()) == false){
        std::cout << "[SERVER] Error when parsing message from server" << std::endl;
        return ;
    }

    // Parsing
    if(message.connectedclients_size() > 0){
        // broadcast with connected clients
        QList<QString> list;
        for(int i = 0; i < message.connectedclients_size(); i++){
            list.push_back(QString(message.connectedclients(i).c_str()));
        }

        emit onlineUsers(list);
    }
    else{
        // rcvd message
        emit upcomingMessage(QString(message.servermessage().c_str()), QString(message.sender().c_str()), message.isprivatesender());
    }
}
