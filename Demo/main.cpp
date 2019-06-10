/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "ApiTest.h"
#include <QCoreApplication>
#include <QThread>

using namespace AeaQt;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ApiTest *apiTest = new ApiTest();
//    apiTest->downloadOneMusic("稻香");
    apiTest->exec();
    return a.exec();
}
