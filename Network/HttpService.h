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
                     const QObject *respReceiver, const char *respReceiverSlot,
                     const QObject *errorReceiver, const char *slotErrorReceiver);

private:
    QNetworkAccessManager m_networkAccessManager;
};

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    HttpRequest(QNetworkReply* reply)
        : QObject(reply)
    {
        auto finishedOperation = [=]()
        {
            if (reply == NULL || reply->error() != QNetworkReply::NoError)
                return;

            emit finished(QVariant::fromValue(reply));
            reply->deleteLater();
        };

        auto erroredOperation = [=](QNetworkReply::NetworkError error)
        {
            emit errored(QVariant::fromValue(error));
            reply->deleteLater();
        };

        connect(reply, &QNetworkReply::finished, finishedOperation);
        connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), erroredOperation);
    }

    void onResponse(const QObject *respReceiver, const char *respReceiverSlot) {
        if (respReceiver != NULL && !QString(respReceiverSlot).isEmpty())
            connect(this, SIGNAL(finished(QVariant)), respReceiver, respReceiverSlot, Qt::UniqueConnection);
    }

    void onError(const QObject *errorReceiver, const char *slotErrorReceiver) {
        if (errorReceiver != NULL && !QString(slotErrorReceiver).isEmpty())
            connect(this, SIGNAL(errored(QVariant)), errorReceiver, slotErrorReceiver, Qt::UniqueConnection);
    }

signals:
    void finished(const QVariant result);
    void errored(const QVariant result);
};

class Test : public QObject {
    Q_OBJECT
public:
    Test()
    {
    }

    void execute()
    {
        m_httpService.get("http://www.aeagean.com")
                   .onResponse(this, SLOT(finish(QVariant)))
                   .exec();

        m_httpService.get("hddttp://www.baidu.com")
                   .onResponse(this, SLOT(finish(QVariant)))
                   .onError(this, SLOT(error(QVariant)))
                   .exec();
    }

private slots:
    void finish(QVariant result)
    {
        QNetworkReply* reply = result.value<QNetworkReply *>();
        qDebug()<<"reply: "<<reply->readAll();
    }

    void error(QVariant result) {
        qDebug()<<"error: "<<result;
    }

private:
    HttpService m_httpService;
};

#endif // HTTP_SERVICE_H
