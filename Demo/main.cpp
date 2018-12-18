/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
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
//    apiTest->downloadOneMusic("稻香");
    apiTest->exec();
    return a.exec();
}
