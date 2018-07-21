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
            .onResponse(this, SLOT(finish(QNetworkReply*)))
            .onError(this, SLOT(error(QString)))
            .exec();
}

void ApiTest::finish(QVariantMap result)
{
    this->deleteLater();
    qDebug()<<result;
}

void ApiTest::finish(QNetworkReply *reply)
{
    this->deleteLater();
    qDebug()<<reply->readLine();
}

void ApiTest::error(QString errorString)
{
    qDebug()<<errorString;
}
