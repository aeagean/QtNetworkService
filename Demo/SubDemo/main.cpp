#include "Test1/ApiTest.h"
#include <QCoreApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<ApiTest*> apis;
    for (int i = 0; i < 10; i++) {
        ApiTest *apiTest = new ApiTest();
        apis.append(apiTest);
        apiTest->exec();
    }

    return a.exec();
}
