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

//    QThread::sleep(10);

//    for (int i = 0; i < 10; i++) {
//        delete apis.at(i);
//    }

    return a.exec();
}
