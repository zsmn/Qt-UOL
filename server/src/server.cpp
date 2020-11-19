#include "server.h"

Server::Server(quint16 serverPort)
{
    // Creating server
    _server = new QTcpServer();
    _serverPort = serverPort;

    // Creating signal mapper to map signals
    _signalMapper = new QSignalMapper();

    // Connecting server to port
    if(_server->listen(QHostAddress::Any, _serverPort)){
         std::cout << "[SERVER] Connected to server in port " << _serverPort << std::endl;

         // Whenever a user connects, it will emit this signal
         connect(_server, SIGNAL(newConnection()), this, SLOT(processUser()));
    }
    else{
        std::cout << "[SERVER] Could not connect to server in port " << _serverPort << std::endl;
    }
}

Server::~Server(){
    // Close server
    _server->close();

    // Closing connections with clients
    QList<QTcpSocket *> keys = _clients.keys();
    for(int i = 0; i < keys.size(); i++){
        keys.at(i)->close();
        _clients.remove(keys.at(i));
    }

    // Delete pointers
    delete _server;
    delete _signalMapper;
}

void Server::sendMessage(QString message, QTcpSocket *socket, QString sender, bool isPrivate){
    // Serializing message to protobuf
    ServerMessage msg;

    msg.set_servermessage(message.toStdString());
    msg.set_sender(sender.toStdString());
    msg.set_isprivatesender(isPrivate);

    sendMessage(msg, socket);
}

void Server::sendMessage(ServerMessage message, QTcpSocket *socket){
    // Serializing protobuf message to string
    std::string buffer;
    message.SerializeToString(&buffer);

    // Writing to socket
    if(socket->write(buffer.c_str(), static_cast<qint64>(buffer.length())) == -1){
        std::cout << "[SERVER] Failed to write to client socket: " << socket->errorString().toStdString() << std::endl;
    }
}

void Server::broadcastConnectedClients(){
    // Serializing message to protobuf
    ServerMessage msg;

    // Taking connected clients and removing old ones
    QList<QTcpSocket *> keys = _clients.keys();
    for(int i = 0; i < keys.size(); i++){
        if(keys.at(i)->state() == QAbstractSocket::UnconnectedState){
            _clients.remove(keys.at(i));
        }
        else{
            msg.add_connectedclients(_clients.value(keys.at(i)));
        }
    }

    // Sending to each socket
    for(int i = 0; i < keys.size(); i++){
        if(keys.at(i)->state() != QAbstractSocket::UnconnectedState){
            sendMessage(msg, keys.at(i));
        }
    }
}

void Server::broadcastMessage(QString message, QString sender){
    QList<QTcpSocket *> keys = _clients.keys();
    for(int i = 0; i < keys.size(); i++){
        sendMessage(message, keys.at(i), sender, false);
    }
}

void Server::processUser(){
    // Taking connected client
    QTcpSocket *socket = _server->nextPendingConnection();

    // Connect new client to slot
    connect(socket, SIGNAL(disconnected()), this, SLOT(broadcastConnectedClients()));

    // Inserting in clients list
    _clients.insert(socket, "");

    // Connecting to signal mapper
    connect(socket, SIGNAL(readyRead()), _signalMapper, SLOT(map()));
    _signalMapper->setMapping(socket, socket);
    connect(_signalMapper, SIGNAL(mapped(QObject *)), this, SLOT(processMsg(QObject *)));
}

QTcpSocket* Server::requestReceiver(QString receiverName){
    QList<QTcpSocket *> keys = _clients.keys();
    for(int i = 0; i < keys.size(); i++){
        if(_clients.value(keys.at(i)) == receiverName.toStdString()){
            return keys.at(i);
        }
    }

    return nullptr;
}

int Server::getUsersWithThatName(QString name){
    QList<QTcpSocket*> keys = _clients.keys();
    int qt = 0;
    for(int i = 0; i < keys.size(); i++){
        if(_clients.value(keys.at(i)) == name.toStdString())
            qt++;
    }

    return qt;
}

void Server::processMsg(QObject *object){
    QTcpSocket* socket = qobject_cast<QTcpSocket *>(object);

    // Reading msg
    ClientMessage message;
    QString buffer(socket->readAll());

    // Parsing to protobuf msg
    if(message.ParseFromArray(buffer.toStdString().c_str(), buffer.length()) == false){
        std::cout << "[SERVER] Error when parsing message from client" << std::endl;
        return ;
    }

    // Checking protobuf msg
    if(message.isprivate()){
        /// if is private

        // if contains client name (first msg)
        if(message.clientname() != ""){
            // insert client name

            // avoiding people with same name
            int times = 0;
            QString name = message.clientname().c_str();
            QString fakeName = name;
            while(getUsersWithThatName(fakeName) != 0){
                times++;
                fakeName = name + std::to_string(times+1).c_str();
            }

            // insert and broadcast to all clients
            _clients.insert(socket, fakeName.toStdString());
            broadcastConnectedClients();
        }
        else{
            // search for the private desired user
            QTcpSocket *desiredReceiver = requestReceiver(QString(message.rcvname().c_str()));


            // send msg to him
            if(desiredReceiver != nullptr){
                if(_clients.value(desiredReceiver) != _clients.value(socket)){
                    sendMessage(QString(message.clientmessage().c_str()), socket, "to " + QString(_clients.value(desiredReceiver).c_str()), true);
                    sendMessage(QString(message.clientmessage().c_str()), desiredReceiver, "from " + QString(_clients.value(socket).c_str()), true);
                }
            }
        }
    }
    else{
        /// if is not private (broadcast)
        if(message.clientmessage() != "")
            broadcastMessage(QString(message.clientmessage().c_str()), QString(_clients.value(socket).c_str()));
    }
}
