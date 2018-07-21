#include "Test1/ApiTest.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    for (int i = 0; i < 10; i++) {
        ApiTest *apiTest = new ApiTest();
        apiTest->exec();
    }

    return a.exec();
}
