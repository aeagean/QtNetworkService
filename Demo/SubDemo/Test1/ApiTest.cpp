#include "ApiTest.h"
#include <QDebug>
#include <QTimer>

ApiTest::ApiTest()
{
    m_service = new HttpService();
}

ApiTest::~ApiTest()
{
    qDebug()<<"ApiTest";
}

void ApiTest::exec()
{
    m_service->get("http://www.baidu.com")
            .onResponse(this, SLOT(finish(QVariantMap)))
            .onError(this, SLOT(error(QString)))
            .exec();
}

void ApiTest::finish(QVariantMap result)
{
//    QTimer::singleShot(1000, [=](){ delete m_service; });
    m_service->deleteLater();
    qDebug()<<result;
}

void ApiTest::error(QString errorString)
{
    qDebug()<<errorString;
}
