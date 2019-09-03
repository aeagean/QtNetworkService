#ifndef APITEST_H
#define APITEST_H

#include "HttpClient.h"

class ApiTest : public QObject
{
    Q_OBJECT
public:
    ApiTest();
    virtual ~ApiTest();

    void downloadOneMusic(const QString &name);
    void exec();

private:
    void saveFile(const QString &fileName, QByteArray data);

private slots:
    void finish(QVariantMap result);
    void finish(QNetworkReply *reply);
    void finish(QByteArray result);

    void error(QString errorString);
    void error(QString errorString, QNetworkReply* reply);
    void error(QNetworkReply::NetworkError error);
    void error(QNetworkReply::NetworkError error, QNetworkReply* reply);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    AeaQt::HttpClient m_service;
};

#endif // APITEST_H
