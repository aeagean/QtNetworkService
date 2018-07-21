#ifndef APITEST_H
#define APITEST_H

#include "HttpService.h"

class ApiTest : public QObject
{
    Q_OBJECT
public:
    ApiTest();
    virtual ~ApiTest();

    void exec();

private slots:
    void finish(QVariantMap result);

private:
    HttpService m_service;
};

#endif // APITEST_H
