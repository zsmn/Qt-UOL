#include <QCoreApplication>
#include <src/server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(9999);

    return a.exec();
}
