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
    m_service.get("1https://www.qt.io")
             .onResopnse([](QByteArray result){ qDebug()<<"Result: "<<result; })
             .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
             .exec();

    m_service.get("1https://www.qt.io")
             .onResponse(this, SLOT(finish(QByteArray)))
             .onError(this, SLOT(error(QString)))
             .exec();
}

void ApiTest::finish(QVariantMap result)
{
    qDebug()<<"Result: "<<result;
}

void ApiTest::finish(QNetworkReply *reply)
{
    qDebug()<<"Result: "<<reply->readLine();
}

void ApiTest::finish(QByteArray result)
{
    qDebug()<<"Result: "<<result;
}

void ApiTest::error(QString errorString)
{
    qDebug()<<"Error: "<<errorString;
}

void ApiTest::error(QString errorString, QNetworkReply *reply)
{
    qDebug()<<"Error: "<<reply->error();
}

void ApiTest::error(QNetworkReply::NetworkError error, QNetworkReply *reply)
{
    qDebug()<<"Error: "<<reply->error();
}
