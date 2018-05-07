#include "HttpRequest.h"

HttpRequest::HttpRequest(QNetworkReply* parent)
    : QNetworkReply(parent)
{
    connect(parent, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(parent, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onErrored(QNetworkReply::NetworkError)));
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::onResponse(const QObject *respReceiver, const char *respReceiverSlot) {
    if (respReceiver != NULL && !QString(respReceiverSlot).isEmpty())
        connect(this, SIGNAL(finished(QVariant)), respReceiver, respReceiverSlot, Qt::UniqueConnection);
}

void HttpRequest::onError(const QObject *errorReceiver, const char *slotErrorReceiver) {
    if (errorReceiver != NULL && !QString(slotErrorReceiver).isEmpty())
        connect(this, SIGNAL(errored(QVariant)), errorReceiver, slotErrorReceiver, Qt::UniqueConnection);
}

void HttpRequest::abort()
{

}

void HttpRequest::onFinished()
{
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    if (reply->error() != QNetworkReply::NoError)
        return;

    emit finished(QVariant::fromValue(reply));
    this->deleteLater();
}

void HttpRequest::onErrored(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    emit errored(QVariant::fromValue(reply));
    this->deleteLater();
}

qint64 HttpRequest::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return -1;
}

HttpRequest::HttpRequest()
{

}
