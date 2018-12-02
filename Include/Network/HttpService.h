/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
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

    HttpRequest get(const QString &url);
    HttpRequest post(const QString &url);
    HttpRequest put(const QString &url);

    HttpRequest send(const QString &url, Operation op = GetOperation);
};

#endif // HTTP_SERVICE_H
