#ifndef HTTP_SERVICE_METHOD
#define HTTP_SERVICE_METHOD

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

#include <functional>

class HttpService;

class HttpServiceMethod
{
public:
    virtual ~HttpServiceMethod();
    explicit HttpServiceMethod(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient);

    HttpServiceMethod &url(const QString &url);
    HttpServiceMethod &header(const QString &key, const QString &value);
    HttpServiceMethod &queryParam(const QString &key, const QString &value);
    HttpServiceMethod &jsonBody(const QVariant &jsonBody);
    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     */
    HttpServiceMethod &onResponse(const QObject *respReceiver, const char *slot);
    /*
     * @onError slot support type: void function(QNetworkReply::NetworkErro errorr)
     *
     */
    HttpServiceMethod &onError(const QObject *errorReceiver, const char *slot);

    bool exec();

private:
    HttpServiceMethod();

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_jsonBody;
    QNetworkAccessManager::Operation m_op;
    HttpService *m_httpService;
    QObject *m_respReceiver;
    QString m_respReceiverSlot;
    QObject *m_errorReceiver;
    QString m_errorReceiverSlot;
};

#endif // HTTP_SERVICE_METHOD
