/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "HttpResponse.h"

#include <QRegExp>
#include <QStringList>
#include <QByteArray>
#include <QNetworkConfigurationManager>
#include <QMetaEnum>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

#define T2S(t) (QString(#t).remove(QRegExp("\\s"))) //type to string

#define _exec(target, type, arg) \
        if (target.canConvert<std::function<void (type)> >()) { \
            std::function<void (type)> func = target.value<std::function<void (type)> >(); func(arg); \
        } \
        else

#define _exec2(target, type1, type2, arg1, arg2) \
        if (target.canConvert<std::function<void (type1, type2)> >()) { \
            std::function<void (type1, type2)> func = target.value<std::function<void (type1, type2)> >(); func(arg1, arg2); \
        } else

using namespace AeaQt;

static const QMap<HttpResponse::SupportMethod, QMap<QString, QVariant>> methodParams =
{
    {
        HttpResponse::onResponse_QNetworkReply_A_Pointer,
        {
            {"types", QStringList({T2S(QNetworkReply*)})},
            {"lambda", T2S(std::function<void (QNetworkReply*)>)},
            {"signal", SIGNAL(finished(QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onResponse_QByteArray,
        {
            {"types", QStringList({T2S(QByteArray)})},
            {"lambda", T2S(std::function<void (QByteArray)>)},
            {"signal", SIGNAL(finished(QByteArray))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onResponse_QVariantMap,
        {
            {"types", QStringList({T2S(QVariantMap)})},
            {"lambda", T2S(std::function<void (QVariantMap)>)},
            {"signal", SIGNAL(finished(QVariantMap))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onDownloadProgress_qint64_qint64,
        {
            {"types", QStringList({T2S(qint64), T2S(qint64)})},
            {"lambda", T2S(std::function<void (qint64, qint64)>)},
            {"signal", SIGNAL(downloadProgress(qint64, qint64))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onError_QNetworkReply_To_NetworkError,
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError)})},
            {"lambda", T2S(std::function<void (QNetworkReply::NetworkError)>)},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onError_QString,
        {
            {"types", QStringList({T2S(QString)})},
            {"lambda", T2S(std::function<void (QString)>)},
            {"signal", SIGNAL(error(QString))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer,
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError), T2S(QNetworkReply*)})},
            {"lambda", T2S(std::function<void (QNetworkReply::NetworkError, QNetworkReply*)>)},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError, QNetworkReply*))},
            {"isAutoInfer", true}
        }
    },
    {
        HttpResponse::onError_QString_QNetworkReply_A_Poniter,
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
    /* extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE */
    return (((int)(*member) - '0') & 0x3);
}

HttpResponse::HttpResponse(QNetworkReply *networkReply,
                           const QMultiMap<SupportMethod, QPair<QString, QVariant> > &slotsMap,
                           const int &timeout,
                           bool isBlock)
    : m_networkReply(networkReply),
      m_slotsMap(slotsMap),
      QObject(networkReply)
{
    slotsMapOperation(m_slotsMap);
    new HttpResponseTimeout(networkReply, timeout);

    connect(m_networkReply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(m_networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));

    if (isBlock) {
        QEventLoop loop;
        QObject::connect(m_networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

HttpResponse::~HttpResponse()
{
}

QNetworkReply *HttpResponse::networkReply()
{
    return m_networkReply;
}

void HttpResponse::onFinished()
{
    QNetworkReply *reply = m_networkReply;
    if (reply->error() != QNetworkReply::NoError)
        return;

    if (m_slotsMap.contains(onResponse_QNetworkReply_A_Pointer)) {
        _exec(m_slotsMap.value(onResponse_QNetworkReply_A_Pointer).second, QNetworkReply*, reply) {
            emit finished(reply);
        }
    }
    else if (m_slotsMap.contains((onResponse_QByteArray))) {
        QByteArray result = reply->readAll();

        _exec(m_slotsMap.value((onResponse_QByteArray)).second, QByteArray, result) {
            emit finished(result);
        }

        reply->deleteLater();
    }
    else if (m_slotsMap.contains((onResponse_QVariantMap))) {
        QByteArray result = reply->readAll();
        QVariantMap resultMap = QJsonDocument::fromJson(result).object().toVariantMap();

        _exec(m_slotsMap.value((onResponse_QVariantMap)).second, QVariantMap, resultMap){
            emit finished(resultMap);
        }

        reply->deleteLater();
    }
}

void HttpResponse::onError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = m_networkReply;
    const QMetaObject & metaObject = QNetworkReply::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NetworkError"));
    QString errorString = reply->errorString().isEmpty() ? metaEnum.valueToKey(error) : reply->errorString();

    if (m_slotsMap.contains((onError_QString_QNetworkReply_A_Poniter))) {
        _exec2(m_slotsMap.value((onError_QString_QNetworkReply_A_Poniter)).second, QString, QNetworkReply*, errorString, reply) {
            emit this->error(errorString, reply);
        }
    }
    else if (m_slotsMap.contains((onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer))) {
        _exec2(m_slotsMap.value((onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer)).second,
              QNetworkReply::NetworkError, QNetworkReply*,
              error, reply) {
            emit this->error(error, reply);
        }
    }
    else if (m_slotsMap.contains((onError_QString))) {
        _exec(m_slotsMap.value((onError_QString)).second, QString, errorString) {
            emit this->error(errorString);
        }

        reply->deleteLater();
    }
    else if (m_slotsMap.contains((onError_QNetworkReply_To_NetworkError))) {
        _exec(m_slotsMap.value((onError_QNetworkReply_To_NetworkError)).second, QNetworkReply::NetworkError, error) {
            emit this->error(error);
        }

        reply->deleteLater();
    }
}

void HttpResponse::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_slotsMap.contains((onDownloadProgress_qint64_qint64))) {
        _exec2(m_slotsMap.value((onDownloadProgress_qint64_qint64)).second, qint64, qint64, bytesReceived, bytesTotal) {
            emit downloadProgress(bytesReceived, bytesTotal);
        }
    }
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
static HttpResponse::SupportMethod getSupportMethod(const QPair<QString, QVariant> &slotMap) {

    QMapIterator<HttpResponse::SupportMethod, QMap<QString, QVariant>> iter(methodParams);

    QString receiverSlot = slotMap.first;
    QString slot;
    QStringList slotTypes;
    extractSlot(receiverSlot, slot, slotTypes);

    while (iter.hasNext()) {
        iter.next();
        HttpResponse::SupportMethod supportMethod = iter.key();
        QMap<QString, QVariant> value = iter.value();
        if (slotTypes == value.value("types").toStringList()) {
            return supportMethod;
        }
        else if (receiverSlot == value.value("lambda").toString()) {
            return supportMethod;
        }
    }

    return HttpResponse::Invalid;
}

static void autoInfterConvertedSupportMethod(QMultiMap<HttpResponse::SupportMethod, QPair<QString, QVariant> > &unconvertedSlotsMap)
{
    QMultiMap<HttpResponse::SupportMethod, QPair<QString, QVariant> > convertedSlotsMap;
    QMapIterator<HttpResponse::SupportMethod, QPair<QString, QVariant> > iter(unconvertedSlotsMap);

    while (iter.hasNext()) {
        iter.next();
        const HttpResponse::SupportMethod supportMethod = iter.key();
        const QPair<QString, QVariant> slotMap = iter.value();

        if (supportMethod == HttpResponse::AutoInfer) {
            HttpResponse::SupportMethod supportMethod  = getSupportMethod(slotMap);
            if (supportMethod == HttpResponse::Invalid) {
                qDebug()<<"Not find support Method!"<<slotMap.first;
            }
            else {
                if (methodParams[supportMethod].value("isAutoInfer").toBool())
                    convertedSlotsMap.insert(supportMethod, slotMap);
                else
                    qDebug()<<"This type["<<methodParams[supportMethod].value("types").toString()<<"] does not support automatic derivation";
            }
        }
        else {
            if (methodParams[supportMethod].value("isAutoInfer").toBool())
                convertedSlotsMap.insert(supportMethod, slotMap);
            else
                qDebug()<<"This type["<<methodParams[supportMethod].value("types").toString()<<"] does not support automatic derivation";
        }
    }

    unconvertedSlotsMap = convertedSlotsMap;

}

void HttpResponse::slotsMapOperation(QMultiMap<SupportMethod, QPair<QString, QVariant> > &slotsMap)
{
    autoInfterConvertedSupportMethod(slotsMap);

    QMapIterator<SupportMethod, QPair<QString, QVariant> > iter(slotsMap);
    while (iter.hasNext()) {
        iter.next();
        SupportMethod supportMethod = iter.key();
        const QPair<QString, QVariant> &slotMap = iter.value();

        const QString &receiverSlot = slotMap.first;
        QVariant target = slotMap.second;
        const QObject *receiver = target.value<QObject*>();

        if (receiver) {
            if (methodParams.contains(supportMethod)) {
                connect(this,
                        methodParams[supportMethod].value("signal").toString().toStdString().data(),
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
