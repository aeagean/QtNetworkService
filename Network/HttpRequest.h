#ifndef HTTP_SERVICE_METHOD
#define HTTP_SERVICE_METHOD

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include <functional>

class HttpService;

class HttpRequest
{
public:
    enum SupportReflexMethod {
        onResponseMethod,
        onErrorMethod
    };

    virtual ~HttpRequest();
    explicit HttpRequest(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient);

    HttpRequest &url(const QString &url);
    HttpRequest &header(const QString &key, const QString &value);
    HttpRequest &headers(const QMap<QString, QString> &headers);

    HttpRequest &queryParam(const QString &key, const QString &value);
    HttpRequest &queryParams(const QMap<QString, QString> &params);

    HttpRequest &jsonBody(const QVariant &jsonBody);
    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     */
    HttpRequest &onResponse(const QObject *respReceiver, const char *slot);
    /*
     * @onError slot support type: void function(QNetworkReply::NetworkErro errorr)
     *
     */
    HttpRequest &onError(const QObject *errorReceiver, const char *slot);

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
