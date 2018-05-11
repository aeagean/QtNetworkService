#include "HttpRequest.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#define TYPE_TO_STRING(t) QString(#t)
#define NUMBER_TO_STRING(n)  QString::number(n)

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

HttpRequest::HttpRequest(QNetworkReply *parent, const QMap<QString, QMap<QString, const QObject *> > &slotsMap)
    : QNetworkReply(parent)
{
    connect(parent, &QNetworkReply::finished, [=]() {
        if (parent->error() == QNetworkReply::NoError)
            slotsMapOperation(slotsMap, HttpServiceMethod::onResponseMethod);
    });

    connect(parent, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [=]() { slotsMapOperation(slotsMap, HttpServiceMethod::onErrorMethod); });
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
        QMetaObject::invokeMethod((QObject *)receiver, slot.toStdString().data(), Q_ARG(QByteArray, reply->readAll()));
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

void HttpRequest::slotsMapOperation(const QMap<QString, QMap<QString, const QObject *> > &slotsMap,
                                    HttpServiceMethod::SupportReflexMethod supportReflexMethod)
{
    QMapIterator<QString, QMap<QString, const QObject *> > iter(slotsMap);
    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, const QObject *> &slotMap = iter.value();

        if (!key.compare(NUMBER_TO_STRING(supportReflexMethod)))
            initRequest(slotMap.first(), slotMap.firstKey().toStdString().data());
    }

    QNetworkReply *reply = (QNetworkReply *)this->parent();
    reply->deleteLater();
}

HttpRequest::HttpRequest()
{

}
