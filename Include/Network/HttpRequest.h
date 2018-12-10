/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "HttpResponse.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

namespace AeaQt {

class HttpService;

class HttpRequest
{
public:
    explicit HttpRequest(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient);
    virtual ~HttpRequest();

    HttpRequest &url(const QString &url);
    HttpRequest &header(const QString &key, const QVariant &value);
    HttpRequest &headers(const QMap<QString, QVariant> &headers);

    HttpRequest &queryParam(const QString &key, const QVariant &value);
    HttpRequest &queryParams(const QMap<QString, QVariant> &params);

    /* Mainly used for identification */
    HttpRequest &userAttribute(const QVariant &value);

    HttpRequest &jsonBody(const QVariant &jsonBody);
    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     * note: The same type is only triggered once
     */
    HttpRequest &onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type = HttpResponse::AutoInfer);
    HttpRequest &onResopnse(std::function<void (QNetworkReply*)> lambda);
    HttpRequest &onResopnse(std::function<void (QVariantMap)> lambda);
    HttpRequest &onResopnse(std::function<void (QByteArray)> lambda);
    HttpRequest &onResopnse(std::function<void (qint64, qint64)> lambda);
    /*
     * @onError slot support type: void function(QNetworkReply::NetworkError error)
     *                             void function(QString errorString);
     *                             void function(QNetworkReply::NetworkError error, QNetworkReply* reply);
     *                             void function(QString errorString, QNetworkReply* reply);
     * note: The same type is only triggered once
     */
    HttpRequest &onError(const QObject *receiver, const char *slot);
    HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    HttpRequest &onError(std::function<void (QString)> lambda);
    HttpRequest &onError(std::function<void (QNetworkReply::NetworkError, QNetworkReply*)> lambda);
    HttpRequest &onError(std::function<void (QString, QNetworkReply*)> lambda);

    HttpResponse *exec();

private:
    HttpRequest();
    HttpRequest &onResopnse(QVariant lambda);

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_jsonBody;
    QNetworkAccessManager::Operation m_op;
    HttpService *m_httpService;
    QMultiMap<QString, QMap<QString, QVariant>> m_slotsMap;
};

}
#endif // HTTP_REQUEST_H
