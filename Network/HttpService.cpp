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

HttpServiceMethod HttpService::get(const QString url)
{
    HttpServiceMethod httpServiceMethod(QNetworkAccessManager::GetOperation, this);
    return httpServiceMethod.url(url);
}

HttpServiceMethod HttpService::post(const QString url)
{
    HttpServiceMethod httpServiceMethod(QNetworkAccessManager::PostOperation, this);
    return httpServiceMethod.url(url);
}
