#include "Test.h"

Test::Test()
{
}

void Test::execute()
{
    m_httpService.get("http://www.aeagean.com")
            .onResponse(this, SLOT(finish(QVariantMap)))
            .onResponse(this, SLOT(finish(QByteArray)))
            .onError(this, SLOT(error(QNetworkReply::NetworkError)))
            .exec();

//    m_httpService.get("http://www.baidu.com")
//            .onResponse(this, SLOT(finish(QVariant)))
//            .onError(this, SLOT(error(QVariant)))
//            .exec();
}

void Test::finish(QVariantMap result)
{
    qDebug()<<"reply: "<<result;
}

void Test::finish(QByteArray result)
{
    qDebug()<<"reply(type: QByteArray): "<<result;
}

void Test::error(QNetworkReply::NetworkError result) {
    qDebug()<<"error: "<<result;
}
