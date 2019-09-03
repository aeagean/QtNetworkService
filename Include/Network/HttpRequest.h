/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "HttpResponse.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

namespace AeaQt {

#ifdef QT_APP_DEBUG
#include <QDebug>
#define debugger qDebug()<<"[AeaQt::Network]"<<__func__
#else
#define debug QString()
#endif

class HttpService;

class HttpRequest
{
public:
    enum BodyType {
        None = 0, // This request does not have a body.
        X_Www_Form_Urlencoded, // x-www-form-urlencoded
        Raw_Text_Json, // application/json
    };

    explicit HttpRequest(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient);
    virtual ~HttpRequest();

    HttpRequest &url(const QString &url);
    HttpRequest &header(const QString &key, const QVariant &value);
    HttpRequest &headers(const QMap<QString, QVariant> &headers);

    HttpRequest &queryParam(const QString &key, const QVariant &value);
    HttpRequest &queryParams(const QMap<QString, QVariant> &params);

    /* Mainly used for identification */
    HttpRequest &userAttribute(const QVariant &value);

    /**
     * @note Will be removed in the future
     */
    HttpRequest &jsonBody(const QVariant &jsonBody);

    HttpRequest &body(const QVariant &body, const BodyType &type = X_Www_Form_Urlencoded);

    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     * note: The same type is only triggered once
     */
    HttpRequest &onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type = HttpResponse::AutoInfer);
    HttpRequest &onResponse(std::function<void (QNetworkReply*)> lambda);
    HttpRequest &onResponse(std::function<void (QVariantMap)> lambda);
    HttpRequest &onResponse(std::function<void (QByteArray)> lambda);
    HttpRequest &onResponse(std::function<void (qint64, qint64)> lambda);
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

    /**
     * @brief msec <= 0, disable timeout
     *        msec >  0, enable timeout
     */
    HttpRequest &timeout(const int &msec = -1);

    HttpResponse *exec();

private:
    HttpRequest();
    HttpRequest &onResponse(QVariant lambda);

private:
    QNetworkRequest                  m_networkRequest;
    QJsonObject                      m_jsonBody;
    QByteArray                       m_body;
    QNetworkAccessManager::Operation m_op;
    HttpService                      *m_httpService;
    int                              m_timeout;
    QMultiMap<HttpResponse::SupportMethod, QPair<QString, QVariant>> m_slotsMap;
};

}
#endif // HTTP_REQUEST_H
