#include "Test.h"

Test::Test()
{
}

void Test::execute()
{
    m_httpService.get("http://www.aeagean.com")
            .onResponse(this, SLOT(finish(QVariant)))
            .exec();

//    m_httpService.get("http://www.baidu.com")
//            .onResponse(this, SLOT(finish(QVariant)))
//            .onError(this, SLOT(error(QVariant)))
//            .exec();
}

void Test::finish(QVariant result)
{
    QNetworkReply* reply = result.value<QNetworkReply *>();
    qDebug()<<"reply: "<<reply->readAll();
}

void Test::error(QVariant result) {
    qDebug()<<"error: "<<result;
}
