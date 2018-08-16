#include "ApiTest.h"
#include <QDebug>
#include <QTimer>

ApiTest::ApiTest()
{

}

ApiTest::~ApiTest()
{
    qDebug()<<"ApiTest";
}

void ApiTest::exec()
{
    m_service.get("http://www.baidu.om")
            .onResponse(this, SLOT(finish(QNetworkReply*)))
            .onError(this, SLOT(error(QNetworkReply::NetworkError, QNetworkReply*)))
            .exec();
    static int i = 0;
    qDebug()<<i++;
}

void ApiTest::finish(QVariantMap result)
{
//    this->deleteLater();
    qDebug()<<result;
}

void ApiTest::finish(QNetworkReply *reply)
{
//    this->deleteLater();
    qDebug()<<reply->readLine();
}

void ApiTest::error(QString errorString)
{
    qDebug()<<errorString;
}

void ApiTest::error(QString errorString, QNetworkReply *reply)
{
    qDebug()<<errorString<<reply->error()<<"Error Type";
}

void ApiTest::error(QNetworkReply::NetworkError error, QNetworkReply *reply)
{
    qDebug()<<error<<reply->error()<<"Error";
}
