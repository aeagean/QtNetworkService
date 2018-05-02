#ifndef JSON_HTTP_METHOD_H
#define JSON_HTTP_METHOD_H

#include "JsonHttpClient.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

class JsonHttpMethod : public QObject
{
    Q_OBJECT
public:
    friend class JsonHttpClient;

    JsonHttpMethod();

    JsonHttpMethod &url(const QString url);
    JsonHttpMethod &operation(QNetworkAccessManager::Operation op);
    JsonHttpMethod &header(const QString &key, const QString &value);
    JsonHttpMethod &query(const QString &key, const QString &value); /* params */
    JsonHttpMethod &body(const QJsonObject &body);
    JsonHttpMethod &sendRequest(const QObject *respReceiver, const char *slot);

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_body;
    QNetworkAccessManager::Operation op;
};

#endif
