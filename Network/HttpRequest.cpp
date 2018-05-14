#include "HttpRequest.h"
#include "HttpService.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QBuffer>

#define NUMBER_TO_STRING(n) QString::number(n)

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{
}

HttpRequest::HttpRequest(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient) :
    m_op(op), m_httpService(jsonHttpClient)
{
}

HttpRequest &HttpRequest::url(const QString &url)
{
    m_networkRequest.setUrl(QUrl(url));
    return *this;
}

HttpRequest &HttpRequest::header(const QString &key, const QVariant &value)
{
    if (value.type() == QVariant::Bool) {
        m_networkRequest.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toBool() ? "true" : "false"));
    }
    else {
        m_networkRequest.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toString().toStdString().data()));
    }

    return *this;
}

HttpRequest &HttpRequest::headers(const QMap<QString, QVariant> &headers)
{
   QMapIterator<QString, QVariant> iter(headers);
   while (iter.hasNext()) {
       iter.next();
       header(iter.key(), iter.value());
   }

   return *this;
}

HttpRequest &HttpRequest::jsonBody(const QVariant &jsonBody)
{
    if (jsonBody.type() == QVariant::Map) {
        m_jsonBody = QJsonObject::fromVariantMap(jsonBody.toMap());
    }
    else if (jsonBody.typeName() ==  QMetaType::typeName(QMetaType::QJsonObject)) {
        m_jsonBody = jsonBody.toJsonObject();
    }

    return *this;
}

HttpRequest &HttpRequest::onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type)
{
    m_slotsMap.insert(NUMBER_TO_STRING(type), {{slot, receiver}});
    return *this;
}

HttpRequest &HttpRequest::onError(const QObject *receiver, const char *slot)
{
    return onResponse(receiver, slot);
}

bool HttpRequest::exec()
{
    QNetworkReply* reply = NULL;
    QBuffer* sendBuffer = NULL;
    QJsonObject sendJson = m_jsonBody;
    if (!sendJson.isEmpty()) {
        QByteArray sendByteArray = QJsonDocument(sendJson).toJson();
        sendBuffer = new QBuffer();
        sendBuffer->setData(sendByteArray);
    }

    reply = m_httpService->createRequest(m_op, m_networkRequest, sendBuffer);

    if (reply == NULL && sendBuffer != NULL) {
        sendBuffer->deleteLater();
        return false;
    }
    else if (sendBuffer != NULL) {
        sendBuffer->setParent(reply);
    }

    return new HttpResponse(reply, m_slotsMap);
}

HttpRequest &HttpRequest::queryParam(const QString &key, const QVariant &value)
{
    QUrl url(m_networkRequest.url());
    QUrlQuery urlQuery(url);

    if (value.type() == QVariant::Bool) {
        urlQuery.addQueryItem(key, value.toBool() ? "true" : "false");
    }
    else {
        urlQuery.addQueryItem(key, value.toString());
    }
    url.setQuery(urlQuery);

    m_networkRequest.setUrl(url);
    return *this;
}

HttpRequest &HttpRequest::queryParams(const QMap<QString, QVariant> &params)
{
    QMapIterator<QString, QVariant> iter(params);
    while (iter.hasNext()) {
        iter.next();
        queryParam(iter.key(), iter.value());
    }

    return *this;
}
