#include "ApiTest.h"

ApiTest::ApiTest()
{

}

void ApiTest::exec()
{
    m_service.get("http://www.baidu.com")
            .onResponse(this, SLOT(finish()))
            .exec();
}

void ApiTest::finish()
{
    this->deleteLater();
}
