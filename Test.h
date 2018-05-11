#ifndef TEST_H
#define TEST_H

#include "HttpService.h"

#include <QObject>

class Test : public QObject {
    Q_OBJECT
public:
    Test();

    void execute();

private slots:
    void finish(QVariantMap result);
    void finish(QByteArray result);
    void finish(QNetworkReply *result);
    void error(QNetworkReply::NetworkError result);

private:
    HttpService m_httpService;
};

#endif // TEST_H
