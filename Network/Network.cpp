#include "Network.h"

Network::Network()
{
    QObject::connect(&m_networkAccessManager,
                     SIGNAL(finished(QNetworkReply*)),
                     this,
                     SLOT(serviceRequestFinished(QNetworkReply*))
                     );
}

Network::~Network()
{
    m_networkAccessManager.disconnect();
}

void Network::get(const QString url)
{
    m_networkRequest.setUrl(QUrl(url));
    m_networkAccessManager.get(m_networkRequest);
}

void Network::post(const QString url, const QByteArray &data)
{
    m_networkRequest.setUrl(QUrl(url));
    m_networkAccessManager.post(m_networkRequest, data);
}

QNetworkRequest &Network::networkRequest()
{
    return m_networkRequest;
}

QMap<QString, Network::FNetwork> &Network::getFNetworkMap()
{
    return m_FNetworkMap;
}

void Network::serviceRequestFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
        qDebug()<<reply->errorString();

    QString urlStr = reply->request().url().toString();
    if (m_FNetworkMap.contains(urlStr))
        (this->*m_FNetworkMap[urlStr])(reply);

    reply->deleteLater();
}
