#include "JsonHttpClient.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>

JsonHttpClient::JsonHttpClient()
{
    connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResponse(QNetworkReply*)));
}

JsonHttpClient::~JsonHttpClient()
{
    m_networkAccessManager.disconnect();
}

JsonHttpMethod &JsonHttpClient::get(const QString url)
{
    JsonHttpMethod jsonHttpMethod;
    return jsonHttpMethod.url(url).
                          operation(QNetworkAccessManager::GetOperation);
}

void JsonHttpClient::initRequest(QNetworkRequest *request, const QObject *respReceiver, const char *slot)
{
    Q_ASSERT(respReceiver);
    Q_ASSERT(slot);

    QString requestId = "Request-" + QUuid::createUuid().toString();
    request->setAttribute(QNetworkRequest::User, requestId);

    HttpRequest* requestForResp = new HttpRequest(this);
    requestForResp->setObjectName(requestId);

    connect(requestForResp, SIGNAL(finished(QVariant)), respReceiver, slot);
}

QNetworkAccessManager &JsonHttpClient::networkAccessManager()
{
    return m_networkAccessManager;
}

void JsonHttpClient::onResponse(QNetworkReply *reply)
{
    QString requestId = reply->request().attribute(QNetworkRequest::User).toString();

    HttpRequest *httpRequest = findChild<HttpRequest *>(requestId);
    if (httpRequest != NULL) {
        httpRequest->onResponse(QVariant::fromValue(reply));
        httpRequest->deleteLater();
    }

    reply->deleteLater();
}

