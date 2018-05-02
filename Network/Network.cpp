#include "Network.h"
#include <QUuid>

#define CUSTOMID_NETWORK_UUID ("NetworkRequest" + QUuid::createUuid().toString())

Network::Network()
{
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
}

Network::~Network()
{
    disconnect();
}

void Network::get(QNetworkRequest request, const QObject *replyReceiver, const char *slot)
{
    if (!bindingSignal(request, replyReceiver, slot))
        return;

    m_networkAccessManager.get(request);
}

void Network::post(QNetworkRequest request, const QByteArray &data, const QObject *replyReceiver, const char *slot)
{
    if (!bindingSignal(request, replyReceiver, slot))
        return;

    m_networkAccessManager.post(request, data);
}

void Network::serviceRequestFinished(QNetworkReply *reply)
{
    QString customId = reply->request().attribute(QNetworkRequest::User).toString();

    NetworkRequest* networkRequest = findChild<NetworkRequest *>(customId);
    if (networkRequest != NULL) {
        networkRequest->finished(QVariant::fromValue(reply));
        networkRequest->deleteLater();
    }

    reply->deleteLater();
}

bool Network::bindingSignal(QNetworkRequest &request, const QObject *replyReceiver, const char *slot)
{
    if (replyReceiver == NULL || QString(slot) == "") {
        qDebug()<<"Cant't find a <replyReceiver> or <slot>!";
        return false;
    }

    QString customId = CUSTOMID_NETWORK_UUID;
    request.setAttribute(QNetworkRequest::User, customId);

    NetworkRequest* customNetworkRequest = new NetworkRequest(this);
    customNetworkRequest->setObjectName(customId);
    connect(customNetworkRequest, SIGNAL(_finished(QVariant)), replyReceiver, slot);

    return true;
}
