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
            .exec();
}

void ApiTest::finish(QVariantMap result)
{
    this->deleteLater();
}
