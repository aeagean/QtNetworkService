#include "Test1/ApiTest.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ApiTest *apiTest = new ApiTest();
    apiTest->exec();

    return a.exec();
}
