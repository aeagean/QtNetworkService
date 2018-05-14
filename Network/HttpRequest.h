#ifndef HTTP_SERVICE_METHOD
#define HTTP_SERVICE_METHOD

#include "HttpResponse.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include <functional>

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

    HttpRequest &jsonBody(const QVariant &jsonBody);
    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     * note: The same type is only triggered once
     */
    HttpRequest &onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type = HttpResponse::AutoInfer);

    /*
     * @onError slot support type: void function(QNetworkReply::NetworkErro errorr)
     * note: The same type is only triggered once
     */
    HttpRequest &onError(const QObject *receiver, const char *slot);

    bool exec();

private:
    HttpRequest();

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_jsonBody;
    QNetworkAccessManager::Operation m_op;
    HttpService *m_httpService;
    QMap<QString, QMap<QString, const QObject *>> m_slotsMap;
};

#endif // HTTP_SERVICE_METHOD
