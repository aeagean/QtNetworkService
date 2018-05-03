#ifndef HTTP_SERVICE_METHOD
#define HTTP_SERVICE_METHOD

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>

class HttpService;

class HttpServiceMethod
{
public:
    HttpServiceMethod();
    explicit HttpServiceMethod(QNetworkAccessManager::Operation op, HttpService *jsonHttpClient);

    HttpServiceMethod &url(const QString url);
    HttpServiceMethod &header(const QString &key, const QString &value);
    HttpServiceMethod &queryParam(const QString &key, const QString &value);
    HttpServiceMethod &jsonBody(const QVariant &jsonBody);
    HttpServiceMethod &onResponse(const QObject *respReceiver, const char *slot);

    bool exec();

private:
    QNetworkRequest m_networkRequest;
    QJsonObject m_jsonBody;
    QNetworkAccessManager::Operation m_op;
    HttpService *m_httpService;
    QObject *m_respReceiver;
    QString m_respReceiverSlot;
};

#endif // HTTP_SERVICE_METHOD
