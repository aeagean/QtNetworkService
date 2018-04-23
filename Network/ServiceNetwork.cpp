#include "ServiceNetwork.h"

#define BAIDU "http://www.baidu.com"
#define AEAGEAN "http://www.aeagean.com"

ServiceNetwork::ServiceNetwork()
{
   get(QNetworkRequest(QUrl(AEAGEAN)), this, SLOT(update(QVariant)));
   get(QNetworkRequest(QUrl(BAIDU)), this, SLOT(update(QVariant)));
}

void ServiceNetwork::update(QVariant data)
{
    QNetworkReply* reply = data.value<QNetworkReply *>();
    qDebug()<<reply->readAll();
}
