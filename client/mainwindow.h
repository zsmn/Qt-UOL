#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <src/client.h>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Client *client;
    QString destiny;

public slots:
    void makeConnection();
    void sendMessage();
    void receiveMessage(QString message, QString sender, bool isPrivate);
    void receiveOnlineUsers(QList<QString> onlineUsers);
    void updateReceiver(QListWidgetItem* item);
};
#endif // MAINWINDOW_H
