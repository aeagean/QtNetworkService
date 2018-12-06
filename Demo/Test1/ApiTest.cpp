/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
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
    m_service.get("http://www.baidu.com")
//            .onResponse(this, SLOT(finish(QNetworkReply*)))
//            .onResopnse([](QNetworkReply *result){qDebug()<<result->readLine();})
            .onResopnse([](QByteArray result){qDebug()<<result;})
            .onError(this, SLOT(error(QNetworkReply::NetworkError, QNetworkReply*)))
            .exec();
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
