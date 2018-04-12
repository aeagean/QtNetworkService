#include <QCoreApplication>
#include "ServiceNetwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServiceNetwork* serviceNetwork = new ServiceNetwork();
//    serviceNetwork->

    return a.exec();
}
