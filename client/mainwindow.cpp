#include "mainwindow.h"
#include <build/ui_mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = nullptr;
    connect(ui->connectButton, SIGNAL(released()), this, SLOT(makeConnection()));
    connect(ui->sendButton, SIGNAL(released()), this, SLOT(sendMessage()));

    connect(ui->connectedUsers, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateReceiver(QListWidgetItem *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeConnection(){
    if(client == nullptr){
        // delete older client
        delete client;

        // connect
        client = new Client(ui->ip->toPlainText(), static_cast<quint16>(ui->port->toPlainText().toInt()), ui->username->toPlainText());
        connect(client, SIGNAL(upcomingMessage(QString, QString, bool)), this, SLOT(receiveMessage(QString, QString, bool)));
        connect(client, SIGNAL(onlineUsers(QList<QString>)), this, SLOT(receiveOnlineUsers(QList<QString>)));

        destiny = "Broadcast";
    }
}

void MainWindow::sendMessage(){
    if(client != nullptr){
        client->sendMsg(ui->text->toPlainText(), destiny);
    }

    ui->text->setPlainText(QString(""));
}

void MainWindow::receiveMessage(QString message, QString sender, bool isPrivate){
    ui->text_2->appendPlainText((isPrivate ? "(private)" : "") + sender + ": " + message);
}

void MainWindow::receiveOnlineUsers(QList<QString> onlineUsers){
    ui->connectedUsers->clear();

    ui->connectedUsers->addItem(new QListWidgetItem(QIcon(":/assets/broadcast.png"), "Broadcast"));

    for(int x = 0; x < onlineUsers.size(); x++){
        ui->connectedUsers->addItem(new QListWidgetItem(QIcon(":/assets/online.png"), onlineUsers.at(x)));
    }
}

void MainWindow::updateReceiver(QListWidgetItem *item){
    destiny = item->text();
    if(destiny == "Broadcast"){
        ui->sendButton->setText("Broadcast");
    }
    else{
        ui->sendButton->setText("Send to " + destiny);
    }
}
