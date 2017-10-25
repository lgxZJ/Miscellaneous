#include <QCoreApplication>
#include "FakeServer.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (FakeServer::start() == false)
        a.quit();
    return a.exec();
}
