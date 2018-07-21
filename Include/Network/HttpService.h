#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpService : public QNetworkAccessManager
{
    Q_OBJECT
public:
    friend class HttpRequest;

    HttpService();
    ~HttpService();

    HttpRequest get(const QString url);
    HttpRequest post(const QString url);
    HttpRequest put(const QString url);
};

#endif // HTTP_SERVICE_H
