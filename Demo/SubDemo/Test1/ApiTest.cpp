#include "ApiTest.h"
#include <QDebug>

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
            .onResponse(this, SLOT(finish(QVariantMap)))
            .onError(this, SLOT(error(QString)))
            .exec();
}

void ApiTest::finish(QVariantMap result)
{
    qDebug()<<result;
    this->deleteLater();
}

void ApiTest::error(QString errorString)
{
    qDebug()<<errorString;
}
