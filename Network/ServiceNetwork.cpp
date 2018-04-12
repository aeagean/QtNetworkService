#include "ServiceNetwork.h"

#define BAIDU "http://www.baidu.com"
#define AEAGEAN "http://www.aeagean.com"

ServiceNetwork::ServiceNetwork()
{
   QMap<QString, Network::FNetwork> &funcMap = getFNetworkMap();
   funcMap[AEAGEAN] = FNetwork(&ServiceNetwork::update1);
   funcMap[BAIDU] = FNetwork(&ServiceNetwork::update2);

   get(AEAGEAN);
   get(BAIDU);
}

void ServiceNetwork::update1(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
}

void ServiceNetwork::update2(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
}
