#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "HttpServiceMethod.h"
#include "HttpRequest.h"
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpService : public QNetworkAccessManager
{
    Q_OBJECT
public:
    friend class HttpServiceMethod;

    HttpService();
    ~HttpService();

    HttpServiceMethod get(const QString url);
    HttpServiceMethod post(const QString url);

protected:
    bool sendRequest(QNetworkAccessManager::Operation op, QNetworkRequest &request, const QVariant &data,
                     const QObject *respReceiver, const char *respReceiverSlot,
                     const QObject *errorReceiver, const char *slotErrorReceiver);
};

#endif // HTTP_SERVICE_H
