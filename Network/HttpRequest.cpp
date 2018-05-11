#include "HttpRequest.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#define TYPE_TO_STRING(t) QString(#t)

static const QStringList supportSlotTypeList = {
    TYPE_TO_STRING(QVariantMap),
    TYPE_TO_STRING(QByteArray),
    TYPE_TO_STRING(QNetworkReply*),
    TYPE_TO_STRING(QNetworkReply::NetworkError)
};

static int extractCode(const char *member)
{
    // extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE
    return (((int)(*member) - '0') & 0x3);
}

static void extractSlot(const QString &respReceiverSlot, QString &extractSlot, QString &extractSlotType)
{
    QString slot(respReceiverSlot);
    if (extractCode(respReceiverSlot.toStdString().data()) == QSLOT_CODE && !slot.isEmpty()) {
        slot.remove(0, 1);
        int startIndex = slot.indexOf('(');
        int endIndex = slot.indexOf(')');
        Q_ASSERT(startIndex != -1 && endIndex != -1);

        extractSlotType = slot.mid(startIndex+1, endIndex-startIndex-1);
        extractSlot = slot.remove(startIndex, endIndex-startIndex+1);
    }
}

HttpRequest::HttpRequest(QNetworkReply* parent,
                         const QObject *respReceiver, const char *respReceiverSlot,
                         const QObject *errorReceiver, const char *errorReceiverSlot)
    : QNetworkReply(parent)
{
    QString respReceiverSlotString(respReceiverSlot);
    connect(parent, &QNetworkReply::finished, [=]() {
        if (parent->error() == QNetworkReply::NoError) {
            initRequest(respReceiver, respReceiverSlotString.toStdString().data());
            parent->deleteLater();
        }
    });

    QString errorReceiverSlotString(errorReceiverSlot);
    connect(parent, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=]() {
        initRequest(errorReceiver, errorReceiverSlotString.toStdString().data());
        parent->deleteLater();
    });
}

HttpRequest::~HttpRequest()
{
    qDebug()<<"aaaa";
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

void HttpRequest::initRequest(const QObject *receiver, const char *receiverSlot)
{
    QString slot;
    QString slotType;
    extractSlot(receiverSlot, slot, slotType);

    QNetworkReply *reply = (QNetworkReply *)this->parent();

    if (slotType == TYPE_TO_STRING(QVariantMap)) {
        QVariantMap resultMap = QJsonDocument::fromJson(reply->readAll()).object().toVariantMap();
        QMetaObject::invokeMethod((QObject *)receiver, slot.toStdString().data(), Q_ARG(QVariantMap, resultMap));
    }
    else if (slotType == TYPE_TO_STRING(QByteArray)) {
        QByteArray resultData = reply->readAll();
        QMetaObject::invokeMethod((QObject *)receiver, slot.toStdString().data(), Q_ARG(QByteArray, resultData));
    }
    else if (slotType == TYPE_TO_STRING(QNetworkReply*) || (slotType.remove(QRegExp("\\s")) == TYPE_TO_STRING(QNetworkReply*))) {
        QMetaObject::invokeMethod((QObject *)receiver, slot.toStdString().data(), Q_ARG(QNetworkReply*, reply));

    }
    else if (slotType == TYPE_TO_STRING(QNetworkReply::NetworkError)) {
        QNetworkReply::NetworkError resultError = reply->error();
        QMetaObject::invokeMethod((QObject *)receiver, slot.toStdString().data(), Q_ARG(QNetworkReply::NetworkError, resultError));

    }
    else {
        qDebug()<<"Don't support type: "<<slotType;
    }
}

HttpRequest::HttpRequest()
{

}
