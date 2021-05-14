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
#include <QDebug>

using namespace AeaQt;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpClient client;
    client.get("https://qthub.com")
          .onResponse([](QByteArray data){ qDebug() << data.left(100); })
          .onError([](QString error){ qDebug() << error; })
          .exec();
    return a.exec();
}
