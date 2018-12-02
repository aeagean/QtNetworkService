/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#include "ApiTest.h"
#include <QCoreApplication>
#include <QThread>

using namespace AeaQt;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ApiTest *apiTest = new ApiTest();
    for (int i = 0; i < 1000; i++) {
        apiTest->exec();
    }
    return a.exec();
}
