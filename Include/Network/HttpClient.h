/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include <QNetworkRequest>
#include <QNetworkReply>

namespace AeaQt {

class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    friend class HttpRequest;

    HttpClient();
    ~HttpClient();

    HttpRequest get(const QString &url);
    HttpRequest post(const QString &url);
    HttpRequest put(const QString &url);

    HttpRequest send(const QString &url, Operation op = GetOperation);
};

}
#endif
