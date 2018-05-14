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
    void finish(int value);
    void error(QNetworkReply::NetworkError result);
    void error(QString errorString);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    HttpService m_httpService;
};

#endif // TEST_H
