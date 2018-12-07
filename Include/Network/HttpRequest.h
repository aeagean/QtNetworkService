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

#include <functional>

template<typename T>
struct isFunctor : std::false_type {
};
template<typename L, typename R, typename... Args>
struct isFunctor<R (L::*)(Args...)> : std::true_type {
};
template<typename L>
struct isLambda : isFunctor<decltype(&L::operator())> {
};

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

//    HttpRequest &onResopnse(std::function<void (QNetworkReply*)> lambda);
//    HttpRequest &onResopnse(std::function<void (QVariantMap)> lambda);
//    HttpRequest &onResopnse(std::function<void (QByteArray)> lambda);

    template<typename L> HttpRequest &onResopnse(L lambda) {
//        QVariant::fromValue(lambda);
//        std::function<void ()> aa = static_cast<std::function<void ()>>(lambda);

//        qDebug()<<aa<<"dlfjslfjsldfj";
//        QVariant::fromValue(lambda);
            lambda();
    }
    /*
     * @onError slot support type: void function(QNetworkReply::NetworkError error)
     *                             void function(QString errorString);
     *                             void function(QNetworkReply::NetworkError error, QNetworkReply* reply);
     *                             void function(QString errorString, QNetworkReply* reply);
     * note: The same type is only triggered once
     */
    HttpRequest &onError(const QObject *receiver, const char *slot);

    HttpResponse *exec();

private:
    HttpRequest();

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_jsonBody;
    QNetworkAccessManager::Operation m_op;
    HttpService *m_httpService;
    QMultiMap<QString, QMap<QString, QVariant>> m_slotsMap;
};

}
#endif // HTTP_REQUEST_H
