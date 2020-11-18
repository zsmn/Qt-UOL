#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    isConnected=false;

}

Login::~Login()
{
    delete ui;
}



void Login::on_ConnectButton_clicked()
{
    if(!isConnected){
        QString nome = ui->nomeUser->text();
        QString ip = ui->ipCon->text();
        int porta = ui->portaCon->text().toInt();
        ui->ConnectButton->setText("Disconect!");
        cliente=new Client(ip, porta, nome);
        isConnected=true;
    }else{
        isConnected=false;
    }

}
