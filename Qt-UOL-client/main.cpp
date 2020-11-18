#include <QCoreApplication>

#include <src/client/client.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client("Pedro Jorge");

    return a.exec();
}
