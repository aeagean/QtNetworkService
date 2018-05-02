#ifndef JSON_HTTP_CLIENT_H
#define SON_HTTP_CLIENT_H

#include "JsonHttpMethod.h"
#include <QNetworkRequest>
#include <QNetworkReply>

class JsonHttpClient : public QObject
{
    Q_OBJECT
public:
    JsonHttpClient();
    ~JsonHttpClient();

    JsonHttpMethod &get(const QString url);

protected:
    void initRequest(QNetworkRequest *request, const QObject *respReceiver, const char *slot);
    QNetworkAccessManager &networkAccessManager();

private slots:
    void onResponse(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkAccessManager;
};

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    HttpRequest(QObject *parent = NULL)
        : QObject(parent)
    {
    }

    void onResponse(const QVariant result)
    {
        emit finished(result);
    }

signals:
    void finished(const QVariant result);
};

class Test : public QObject {
    Q_OBJECT
public slots:
    void finish(QVariant result) {
        QNetworkReply* reply = result.value<QNetworkReply *>();
        qDebug()<<reply->readAll();
    }
};
#endif
