/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "HttpService.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QBuffer>

using namespace AeaQt;

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
}

HttpRequest HttpService::get(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::GetOperation, this).url(url);
}

HttpRequest HttpService::post(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::PostOperation, this).url(url);
}

HttpRequest HttpService::put(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::PutOperation, this).url(url);
}

HttpRequest HttpService::send(const QString &url, QNetworkAccessManager::Operation op)
{
    return HttpRequest(op, this).url(url);
}
