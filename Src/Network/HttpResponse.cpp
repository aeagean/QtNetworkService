/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#include "HttpResponse.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkConfigurationManager>
#include <QMetaEnum>

#define exec(target, type, arg) \
        if (target.canConvert<std::function<void (type)> >()) { \
            std::function<void (type)> func = target.value<std::function<void (type)> >(); func(arg); \
        } \
        else

#define exec2(target, type1, type2, arg1, arg2) \
        if (target.canConvert<std::function<void (type1, type2)> >()) { \
            std::function<void (type1, type2)> func = target.value<std::function<void (type1, type2)> >(); func(arg1, arg2); \
        } else

using namespace AeaQt;

static const QMap<QString, QMap<QString, QVariant>> methodParams =
{
    {
        N2S(HttpResponse::onResponse_QNetworkReply_A_Pointer),
        {
            {"types", QStringList({T2S(QNetworkReply*)})},
            {"lambda", T2S(std::function<void (QNetworkReply*)>)},
            {"signal", SIGNAL(finished(QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onResponse_QByteArray),
        {
            {"types", QStringList({T2S(QByteArray)})},
            {"lambda", T2S(std::function<void (QByteArray)>)},
            {"signal", SIGNAL(finished(QByteArray))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onResponse_QVariantMap),
        {
            {"types", QStringList({T2S(QVariantMap)})},
            {"lambda", T2S(std::function<void (QVariantMap)>)},
            {"signal", SIGNAL(finished(QVariantMap))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onDownloadProgress_qint64_qint64),
        {
            {"types", QStringList({T2S(qint64), T2S(qint64)})},
            {"lambda", T2S(std::function<void (qint64, qint64)>)},
            {"signal", SIGNAL(downloadProgress(qint64, qint64))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QNetworkReply_To_NetworkError),
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError)})},
            {"lambda", T2S(std::function<void (QNetworkReply::NetworkError)>)},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QString),
        {
            {"types", QStringList({T2S(QString)})},
            {"lambda", T2S(std::function<void (QString)>)},
            {"signal", SIGNAL(error(QString))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer),
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError), T2S(QNetworkReply*)})},
            {"lambda", T2S(std::function<void (QNetworkReply::NetworkError, QNetworkReply*)>)},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError, QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        N2S(HttpResponse::onError_QString_QNetworkReply_A_Poniter),
        {
            {"types", QStringList({T2S(QString), T2S(QNetworkReply*)})},
            {"lambda", T2S(std::function<void (QString, QNetworkReply*)>)},
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

HttpResponse::HttpResponse(QNetworkReply *parent, const QMultiMap<QString, QMap<QString, QVariant> > &slotsMap)
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
    if (reply->error() != QNetworkReply::NoError)
        return;

    if (m_slotsMap.contains(N2S(onResponse_QNetworkReply_A_Pointer))) {
        exec(m_slotsMap.value(N2S(onResponse_QNetworkReply_A_Pointer)).first(), QNetworkReply*, reply) {
            emit finished(reply);
        }
    }
    else if (m_slotsMap.contains(N2S(onResponse_QByteArray))) {
        QByteArray result = reply->readAll();

        exec(m_slotsMap.value(N2S(onResponse_QByteArray)).first(), QByteArray, result) {
            emit finished(result);
        }

        reply->deleteLater();
    }
    else if (m_slotsMap.contains(N2S(onResponse_QVariantMap))) {
        QByteArray result = reply->readAll();
        QVariantMap resultMap = QJsonDocument::fromJson(result).object().toVariantMap();

        exec(m_slotsMap.value(N2S(onResponse_QVariantMap)).first(), QVariantMap, resultMap){
            emit finished(resultMap);
        }

        reply->deleteLater();
    }
}

void HttpResponse::onError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    const QMetaObject & metaObject = QNetworkReply::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NetworkError"));
    QString errorString = reply->errorString().isEmpty() ? metaEnum.valueToKey(error) : reply->errorString();

    if (m_slotsMap.contains(N2S(onError_QString_QNetworkReply_A_Poniter))) {
        exec2(m_slotsMap.value(N2S(onError_QString_QNetworkReply_A_Poniter)).first(), QString, QNetworkReply*, errorString, reply) {
            emit this->error(errorString, reply);
        }
    }
    else if (m_slotsMap.contains(N2S(onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer))) {
        exec2(m_slotsMap.value(N2S(onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer)).first(),
              QNetworkReply::NetworkError, QNetworkReply*,
              error, reply) {
            emit this->error(error, reply);
        }
    }
    else if (m_slotsMap.contains(N2S(onError_QString))) {
        exec(m_slotsMap.value(N2S(onError_QString)).first(), QString, errorString) {
            emit this->error(errorString);
            reply->deleteLater();
        }
    }
    else if (m_slotsMap.contains(N2S(onError_QNetworkReply_To_NetworkError))) {
        exec(m_slotsMap.value(N2S(onError_QNetworkReply_To_NetworkError)).first(), QNetworkReply::NetworkError, error) {
            emit this->error(error);
            reply->deleteLater();
        }
    }
}

void HttpResponse::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_slotsMap.contains(N2S(onDownloadProgress_qint64_qint64))) {
        exec2(m_slotsMap.value(N2S(onDownloadProgress_qint64_qint64)).first(), qint64, qint64, bytesReceived, bytesTotal) {
            emit downloadProgress(bytesReceived, bytesTotal);
        }
    }
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
static QString getSupportMethod(const QMap<QString, QVariant> &slotMap) {

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
        else if (receiverSlot == value.value("lambda").toString()) {
            return key;
        }
    }

    return "";
}

static void autoInfterConvertedSupportMethod(QMultiMap<QString, QMap<QString, QVariant> > &unconvertedSlotsMap)
{
    QMultiMap<QString, QMap<QString, QVariant> > convertedSlotsMap;
    QMapIterator<QString, QMap<QString, QVariant> > iter(unconvertedSlotsMap);

    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, QVariant> &slotMap = iter.value();

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

void HttpResponse::slotsMapOperation(QMultiMap<QString, QMap<QString, QVariant> > &slotsMap)
{
    autoInfterConvertedSupportMethod(slotsMap);

    QMapIterator<QString, QMap<QString, QVariant> > iter(slotsMap);
    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, QVariant> &slotMap = iter.value();

        const QString &receiverSlot = slotMap.firstKey();
        QVariant target = slotMap.first();
        const QObject *receiver = target.value<QObject*>();

        if (receiver) {
            if (methodParams.contains(key)) {
                connect(this,
                        methodParams[key].value("signal").toString().toStdString().data(),
                        receiver,
                        receiverSlot.toStdString().data(),
                        Qt::QueuedConnection);
            }
        }
    }
}

HttpResponse::HttpResponse()
{

}
