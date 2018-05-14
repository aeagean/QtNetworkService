#include "Test.h"

Test::Test()
{
}

void Test::execute()
{
    m_httpService.get("http://laundromat-temp-file.oss-cn-hangzhou.aliyuncs.com/CycleUpdate/0000.db.13620/BVL3D120TY6/春秋.png")
//            .onResponse(this, SLOT(finish(QVariantMap)))
//            .onResponse(this, SLOT(finish(QByteArray)))
            .onResponse(this, SLOT(finish(QNetworkReply *)))
            .onResponse(this, SLOT(downloadProgress(qint64, qint64)))
//            .onResponse(this, SLOT(error(QNetworkReply::NetworkError)), HttpResponse::AutoInfer)
            .exec();

//    m_httpService.get("http://www.baidu.com")
//            .onResponse(this, SLOT(finish(QByteArray)))
//            .onError(this, SLOT(error(QVariant)))
//            .exec();
}

void Test::finish(QVariantMap result)
{
    qDebug()<<"reply: "<<result;
}

void Test::finish(QByteArray result)
{
    qDebug()<<"reply(QByteArray): "<<QString(result).simplified();
}

void Test::finish(QNetworkReply *result)
{
    qDebug()<<"reply(type(QNetworkReply*): "<<result->readAll();
}

void Test::error(QNetworkReply::NetworkError result) {
    qDebug()<<"error: "<<result;
}

void Test::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"bytesReceived: "<<bytesReceived;
    qDebug()<<"bytesTotal: "<<bytesTotal;
}
