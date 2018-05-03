#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "HttpServiceMethod.h"
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpService : public QObject
{
    Q_OBJECT
public:
    friend class HttpServiceMethod;

    HttpService();
    ~HttpService();

    HttpServiceMethod get(const QString url);
    HttpServiceMethod post(const QString url);

protected:
    bool sendRequest(QNetworkAccessManager::Operation op, QNetworkRequest &request, QVariant data,
                     const QObject *respReceiver, const char *slot);

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
public:
    Test()
    {
    }

    void execute()
    {
        HttpService httpService;
        httpService.get("http://www.aeagean.com")
                   .onResponse(this, SLOT(finish(QVariant)))
                   .exec();

        httpService.get("http://www.baidu.com")
                   .onResponse(this, SLOT(finish(QVariant)))
                   .exec();
    }

public slots:
    void finish(QVariant result)
    {
        QNetworkReply* reply = result.value<QNetworkReply *>();
        qDebug()<<reply->readAll();
    }
};

#endif // HTTP_SERVICE_H
