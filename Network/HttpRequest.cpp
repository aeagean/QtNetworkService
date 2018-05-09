#include "HttpRequest.h"

HttpRequest::HttpRequest(QNetworkReply* parent,
                         const QObject *respReceiver, const char *respReceiverSlot,
                         const QObject *errorReceiver, const char *slotErrorReceiver)
    : QNetworkReply(parent)
{
    QString method(respReceiverSlot);

    qDebug()<<method.remove(QRegExp("(\([^\]]*\))"));

    connect(parent, &QNetworkReply::finished, [=]() {
        QMetaObject::invokeMethod((QObject *)respReceiver, method.toStdString().data(), Q_ARG(QVariant, QVariant::fromValue(parent->readAll())));
        this->deleteLater();
    });
}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::abort()
{

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
