#include "ApiTest.h"
#include <QCoreApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ApiTest *apiTest = new ApiTest();
    for (int i = 0; i < 10000; i++) {
        apiTest->exec();
    }

    return a.exec();
}
