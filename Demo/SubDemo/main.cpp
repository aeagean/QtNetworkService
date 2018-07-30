#include "Test1/ApiTest.h"
#include "Test1/SocketTest.h"
#include <QCoreApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QList<ApiTest*> apis;
//    for (int i = 0; i < 1; i++) {
//        ApiTest *apiTest = new ApiTest();
//        apis.append(apiTest);
//        apiTest->exec();
//    }

    SocketTest udpServer;

    return a.exec();
}
