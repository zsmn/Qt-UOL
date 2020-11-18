#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "src/client/client.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    Client *cliente;
    ~Login();

private slots:

    void on_ConnectButton_clicked();

private:
    Ui::Login *ui;
    bool isConnected;

};

#endif // LOGIN_H
