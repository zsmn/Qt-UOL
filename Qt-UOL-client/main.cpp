#include <QCoreApplication>

#include <src/client/client.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;

    for(int x = 0; x < 5; x++){
        client.sendMsg("str");
    }

    return a.exec();
}
