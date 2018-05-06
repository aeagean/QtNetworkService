#include "HttpService.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
    m_networkAccessManager.disconnect();
}

HttpServiceMethod HttpService::get(const QString url)
{
    HttpServiceMethod httpServiceMethod(QNetworkAccessManager::GetOperation, this);
    return httpServiceMethod.url(url);
}

HttpServiceMethod HttpService::post(const QString url)
{
    HttpServiceMethod httpServiceMethod(QNetworkAccessManager::PostOperation, this);
    return httpServiceMethod.url(url);
}

bool HttpService::sendRequest(QNetworkAccessManager::Operation op, QNetworkRequest &request, QVariant data,
                              const QObject *respReceiver, const char *respReceiverSlot,
                              const QObject *errorReceiver, const char *errorReceiverSlot)
{
    QNetworkReply* reply = NULL;
    if (op == QNetworkAccessManager::GetOperation) {
        reply = m_networkAccessManager.get(request);
    }
    else if (op == QNetworkAccessManager::PostOperation) {
        reply = m_networkAccessManager.post(request, QJsonDocument(data.toJsonObject()).toBinaryData());
    }

    if (reply == NULL)
        return false;

    HttpRequest* requestForResp = new HttpRequest(reply);
    requestForResp->onResponse(respReceiver, respReceiverSlot);
    requestForResp->onError(errorReceiver, errorReceiverSlot);

    return true;
}
