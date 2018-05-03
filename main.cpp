#include "HttpService.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Test* test = new Test;
    test->execute();
    return a.exec();
}
