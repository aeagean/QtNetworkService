#include "HttpService.h"
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QBuffer>

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
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
    QBuffer* sendBuffer = NULL;
    QJsonObject sendJson = data.toJsonObject();
    if (!sendJson.isEmpty()) {
        QByteArray sendByteArray = QJsonDocument(sendJson).toJson();
        sendBuffer = new QBuffer;
        sendBuffer->setData(sendByteArray);
        sendBuffer->setParent(reply);
    }

    reply = QNetworkAccessManager::createRequest(op, request, sendBuffer);

    if (reply == NULL && sendBuffer != NULL) {
        sendBuffer->deleteLater();
        return false;
    }

    HttpRequest* requestForResp = new HttpRequest(reply);
    requestForResp->onResponse(respReceiver, respReceiverSlot);
    requestForResp->onError(errorReceiver, errorReceiverSlot);

    return true;
}
