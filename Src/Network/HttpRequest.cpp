/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "HttpRequest.h"
#include "HttpClient.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QBuffer>
#include <QMetaEnum>

using namespace AeaQt;

static const char *s_httpOperation[] = {
    "UnknownOperation",
    "HeadOperation",
    "GetOperation",
    "PutOperation",
    "PostOperation",
    "DeleteOperation",
    "CustomOperation"
};

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{
}

HttpRequest::HttpRequest(QNetworkAccessManager::Operation op, HttpClient *jsonHttpClient) :
    m_body(QByteArray()),
    m_op(op),
    m_httpService(jsonHttpClient),
    m_timeout(-1)
{
}

HttpRequest &HttpRequest::url(const QString &url)
{
    m_networkRequest.setUrl(QUrl(url));
    return *this;
}

HttpRequest &HttpRequest::header(const QString &key, const QVariant &value)
{
    m_networkRequest.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toString().toStdString().data()));

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

HttpRequest &HttpRequest::body(const QVariantMap &content)
{
    m_body = QJsonDocument(QJsonObject::fromVariantMap(content)).toJson();
    return *this;
}

HttpRequest &HttpRequest::body(const QJsonObject &content)
{
    m_body = QJsonDocument(QJsonObject::fromVariantMap(content.toVariantMap())).toJson();
    return *this;
}

HttpRequest &HttpRequest::body(const QByteArray &content)
{
    m_body = content;
    return *this;
}

#if 0
HttpRequest &HttpRequest::body(const QVariant &body)
{
    /// clear m_jsonBody
    m_jsonBody = QJsonObject();

    if (type == X_Www_Form_Urlencoded) {
        QUrl url;
        QUrlQuery urlQuery(url);

        if (body.type() == QVariant::Map
            || body.typeName() ==  QMetaType::typeName(QMetaType::QJsonObject)) {

            QMapIterator<QString, QVariant> i(body.toMap());
            while (i.hasNext()) {
                i.next();
                urlQuery.addQueryItem(i.key(), i.value().toString());
            }

            url.setQuery(urlQuery);
            m_body = url.toString(QUrl::FullyEncoded).toUtf8().remove(0, 1);
        }
        else {
            m_body = body.toByteArray();
        }
    }
    else if (type == Raw_Text_Json) {
        if (body.type() == QVariant::Map
            || body.typeName() ==  QMetaType::typeName(QMetaType::QJsonObject)) {

            m_body = QJsonDocument(QJsonObject::fromVariantMap(body.toMap())).toJson();
        }
        else {
            warning << "This is not data in JSON format(QVariantMap or QJsonObject).";
            m_body = QByteArray();
            // warning output
        }
    }
    else {
        m_body = QByteArray();
        warning << "Disable body.";
    }

    debugger << "Body Content:" << m_body;
    return *this;
}
#endif

HttpRequest &HttpRequest::onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type)
{
    m_slotsMap.insert(type, {slot, QVariant::fromValue((QObject *)receiver)});
    return *this;
}

HttpRequest &HttpRequest::onResponse(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QVariantMap)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QByteArray)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(const QObject *receiver, const char *slot)
{
    return onResponse(receiver, slot, HttpResponse::AutoInfer);
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QString)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError, QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QString, QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::timeout(const int &msec)
{
    m_timeout = msec;
    return *this;
}

HttpRequest &HttpRequest::block()
{
    m_isBlock = true;
    return *this;
}

HttpRequest &HttpRequest::onResponse(QVariant lambda)
{
    m_slotsMap.insert(HttpResponse::AutoInfer, {lambda.typeName(), lambda});

    return *this;
}

HttpResponse *HttpRequest::exec()
{
    QNetworkReply* reply = NULL;
    QBuffer* sendBuffer = new QBuffer();
    if (! m_body.isEmpty()) {
        sendBuffer->setData(m_body);
    }

    debugger << "Http Client info: ";
    debugger << "Type: " << s_httpOperation[m_op];
    debugger << "Url: " << m_networkRequest.url().toString();
    QString headers;
    for (int i = 0; i < m_networkRequest.rawHeaderList().count(); i++) {
        QString each = m_networkRequest.rawHeaderList().at(i);
        QString header = m_networkRequest.rawHeader(each.toUtf8());
        headers += QString("%1: %2;").arg(each)
                                     .arg(header);
    }
    debugger << "Header: " << headers;
    debugger << "Send buffer(Body):\r\n" << m_body;

    reply = m_httpService->createRequest(m_op, m_networkRequest, sendBuffer);

    if (reply == NULL) {
        sendBuffer->deleteLater();
        return NULL;
    }
    else {
        sendBuffer->setParent(reply);
    }

    return new HttpResponse(reply, m_slotsMap, m_timeout, m_isBlock);
}

HttpRequest &HttpRequest::queryParam(const QString &key, const QVariant &value)
{
    QUrl url(m_networkRequest.url());
    QUrlQuery urlQuery(url);

    urlQuery.addQueryItem(key, value.toString());
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

HttpRequest &HttpRequest::userAttribute(const QVariant &value)
{
    m_networkRequest.setAttribute(QNetworkRequest::User, value);
    return *this;
}
