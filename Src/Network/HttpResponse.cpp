#include "HttpResponse.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkConfigurationManager>
#include <QMetaEnum>

#define TYPE_TO_STRING(t) QString(#t)
#define NUMBER_TO_STRING(n)  QString::number(n)
#define N2S(n) NUMBER_TO_STRING(n)
#define T2S(t) TYPE_TO_STRING(t)

static const QMap<QString, QMap<QString, QVariant>> methodParams =
{
    {
        N2S(HttpResponse::onResponse_QNetworkReply_A_Pointer),
        {
            {"types", QStringList({T2S(QNetworkReply*)})},
            {"signal", SIGNAL(finished(QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onResponse_QByteArray),
        {
            {"types", QStringList({T2S(QByteArray)})},
            {"signal", SIGNAL(finished(QByteArray))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onResponse_QVariantMap),
        {
            {"types", QStringList({T2S(QVariantMap)})},
            {"signal", SIGNAL(finished(QVariantMap))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onDownloadProgress_qint64_qint64),
        {
            {"types", QStringList({T2S(qint64), T2S(qint64)})},
            {"signal", SIGNAL(downloadProgress(qint64, qint64))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QNetworkReply_To_NetworkError),
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError)})},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QString),
        {
            {"types", QStringList({T2S(QString)})},
            {"signal", SIGNAL(error(QString))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer),
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError), T2S(QNetworkReply*)})},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError, QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QString_QNetworkReply_A_Poniter),
        {
            {"types", QStringList({T2S(QString), T2S(QNetworkReply*)})},
            {"signal", SIGNAL(error(QString, QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
};

static int extractCode(const char *member)
{
    // extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE
    return (((int)(*member) - '0') & 0x3);
}

HttpResponse::HttpResponse(QNetworkReply *parent, const QMultiMap<QString, QMap<QString, const QObject *> > &slotsMap)
    : QNetworkReply(parent), m_slotsMap(slotsMap)
{
    slotsMapOperation(m_slotsMap);

    connect(parent, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(parent, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(parent, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));

    if (!QNetworkConfigurationManager().isOnline())
        onError(QNetworkReply::UnknownNetworkError);
}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::abort()
{

}

void HttpResponse::onFinished()
{
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    if (m_slotsMap.contains(N2S(SupportMethod::onResponse_QNetworkReply_A_Pointer))) {
        emit finished(reply);
    }
    else if (m_slotsMap.contains(N2S(SupportMethod::onResponse_QByteArray))) {
        QByteArray result = reply->readAll();
        emit finished(result);
        reply->deleteLater();
    }
    else if (m_slotsMap.contains(N2S(SupportMethod::onResponse_QVariantMap))) {
        QByteArray result = reply->readAll();
        emit finished(QJsonDocument::fromJson(result).object().toVariantMap());
        reply->deleteLater();
    }
}

void HttpResponse::onError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    const QMetaObject & metaObject = QNetworkReply::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NetworkError"));
    QString errorString = reply->errorString().isEmpty() ? metaEnum.valueToKey(error) : reply->errorString();

    if (m_slotsMap.contains(N2S(SupportMethod::onError_QString))) {
        emit this->error(errorString);
        reply->deleteLater();
    }
    else if (m_slotsMap.contains(N2S(SupportMethod::onError_QNetworkReply_To_NetworkError))) {
        emit this->error(error);
        reply->deleteLater();
    }
    else if (m_slotsMap.contains(N2S(SupportMethod::onError_QString_QNetworkReply_A_Poniter))) {
        emit this->error(errorString, reply);
    }
    else if (m_slotsMap.contains(N2S(SupportMethod::onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer))) {
        emit this->error(error, reply);
    }
}

void HttpResponse::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(bytesReceived, bytesTotal);
}

qint64 HttpResponse::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return -1;
}

static void extractSlot(const QString &respReceiverSlot, QString &extractSlot, QStringList &extractSlotTypes)
{
    QString slot(respReceiverSlot);
    if (extractCode(respReceiverSlot.toStdString().data()) == QSLOT_CODE && !slot.isEmpty()) {
        slot.remove(0, 1);

        QString unconvertedSlotType = slot;
        int startIndex = slot.indexOf('(');
        int endIndex = slot.indexOf(')');
        Q_ASSERT(startIndex != -1 && endIndex != -1);

        extractSlot = slot.remove(startIndex, endIndex-startIndex+1);

        extractSlotTypes = unconvertedSlotType.mid(startIndex+1, endIndex-startIndex-1)
                .remove(QRegExp("\\s"))
                .split(',');
    }
}

/* from slotMap get [SupportMethod] */
static QString getSupportMethod(const QMap<QString, const QObject *> &slotMap) {

    QMapIterator<QString, QMap<QString, QVariant>> iter(methodParams);

    QString receiverSlot = slotMap.firstKey();
    QString slot;
    QStringList slotTypes;
    extractSlot(receiverSlot, slot, slotTypes);

    while (iter.hasNext()) {
        iter.next();
        QString key = iter.key();
        QMap<QString, QVariant> value = iter.value();
        if (slotTypes == value.value("types").toStringList()) {
            return key;
        }
    }

    return "";
}

static void autoInfterConvertedSupportMethod(QMultiMap<QString, QMap<QString, const QObject *> > &unconvertedSlotsMap)
{
    QMultiMap<QString, QMap<QString, const QObject *> > convertedSlotsMap;
    QMapIterator<QString, QMap<QString, const QObject *> > iter(unconvertedSlotsMap);

    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, const QObject *> &slotMap = iter.value();

        HttpResponse::SupportMethod supportMethod = (HttpResponse::SupportMethod)key.toInt();
        if (supportMethod == HttpResponse::AutoInfer) {
            QString key = getSupportMethod(slotMap);
            if (key == "") {
                qDebug()<<"Not find support Method!"<<slotMap.firstKey();
            }
            else {
                if (methodParams[key].value("isAutoInfer").toBool())
                    convertedSlotsMap.insert(key, slotMap);
                else
                    qDebug()<<"This type["<<methodParams[key].value("types").toString()<<"] does not support automatic derivation";
            }
        }
        else {
            if (methodParams[key].value("isAutoInfer").toBool())
                convertedSlotsMap.insert(key, slotMap);
            else
                qDebug()<<"This type["<<methodParams[key].value("types").toString()<<"] does not support automatic derivation";
        }
    }

    unconvertedSlotsMap = convertedSlotsMap;

}

void HttpResponse::slotsMapOperation(QMultiMap<QString, QMap<QString, const QObject *> > &slotsMap)
{
    autoInfterConvertedSupportMethod(slotsMap);

    QMapIterator<QString, QMap<QString, const QObject *> > iter(slotsMap);
    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, const QObject *> &slotMap = iter.value();

        const QObject *receiver = slotMap.first();
        const QString &receiverSlot = slotMap.firstKey();

        if (methodParams.contains(key)) {
            connect(this,
                    methodParams[key].value("signal").toString().toStdString().data(),
                    receiver,
                    receiverSlot.toStdString().data(),
                    Qt::QueuedConnection);
        }
    }
}

HttpResponse::HttpResponse()
{

}
