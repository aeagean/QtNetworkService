/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qthub.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "HttpClient.hpp"
#include <QCoreApplication>

using namespace AeaQt;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    ApiTest *apiTest = new ApiTest();
//    apiTest->downloadOneMusic("稻香");
//    apiTest->exec();
    HttpClient c;
    c.get("https://qthub.com")
     .exec();
    return a.exec();
}
