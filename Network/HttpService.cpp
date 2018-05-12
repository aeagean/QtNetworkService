#include "HttpService.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QBuffer>

HttpService::HttpService()
{
}

HttpService::~HttpService()
{
}

HttpRequest HttpService::get(const QString url)
{
    HttpRequest httpRequest(QNetworkAccessManager::GetOperation, this);
    return httpRequest.url(url);
}

HttpRequest HttpService::post(const QString url)
{
    HttpRequest httpRequest(QNetworkAccessManager::PostOperation, this);
    return httpRequest.url(url);
}
