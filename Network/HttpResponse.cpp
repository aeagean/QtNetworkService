#include "HttpResponse.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#define TYPE_TO_STRING(t) QString(#t)
#define NUMBER_TO_STRING(n)  QString::number(n)
#define N2S(n) NUMBER_TO_STRING(n)
#define T2S(t) TYPE_TO_STRING(t)

static const QStringList supportSlotTypeList = {
    TYPE_TO_STRING(QVariantMap),
    TYPE_TO_STRING(QByteArray),
    TYPE_TO_STRING(QNetworkReply*),
    TYPE_TO_STRING(QNetworkReply::NetworkError)
};

static const QMap<QString, QMap<QString, QVariant>> methodParams =
{
    {
        N2S(HttpResponse::onResponse_QNetworkReply_A_Pointer),
        {
            {"types", QStringList({T2S(QNetworkReply*)})},
            {"signal", SIGNAL(finished(QNetworkReply*))}
        }
    },
    {
        N2S(HttpResponse::onResponse_QByteArray),
        {
            {"types", QStringList({T2S(QByteArray)})},
            {"signal", SIGNAL(finished(QByteArray))}
        }
    },
    {
        N2S(HttpResponse::onResponse_QVariantMap),
        {
            {"types", QStringList({T2S(QVariantMap)})},
            {"signal", SIGNAL(finished(QVariantMap))}
        }
    },
    {
        N2S(HttpResponse::onDownloadProgress_qint64_qint64),
        {
            {"types", QStringList({T2S(qint64), T2S(qint64)})},
            {"signal", SIGNAL(downloadProgress(qint64, qint64))}
        }
    },
    {
        N2S(HttpResponse::onError_QNetworkReply_To_NetworkError),
        {
            {"types", QStringList({T2S(QNetworkReply::NetworkError)})},
            {"signal", SIGNAL(error(QNetworkReply::NetworkError))}
        }
    },
    {
        N2S(HttpResponse::onError_QString),
        {
            {"types", QStringList({T2S(QString)})},
            {"signal", SIGNAL(error(QString))}
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
    connect(parent, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] { onError();});
    connect(parent, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
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

    emit finished(reply);
    emit finished(reply->readAll());
    emit finished(QJsonDocument::fromJson(reply->readAll()).object().toVariantMap());
}

void HttpResponse::onError()
{
    QNetworkReply *reply = (QNetworkReply *)this->parent();
    emit error(reply->errorString());
    emit error(reply->error());
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

void HttpResponse::slotsMapOperation(const QMultiMap<QString, QMap<QString, const QObject *> > &slotsMap)
{
    QMap<QString, QMap<QString, const QObject *>> signalsMap = {
        {TYPE_TO_STRING(onResponse_QNetworkReply_A_Pointer), {{SIGNAL(finished(QNetworkReply*)), this}} },
        {TYPE_TO_STRING(onResponse_QByteArray), {{SIGNAL(finished(QByteArray)), this}} },
    };

    QNetworkReply *reply = (QNetworkReply *)this->parent();
    QMapIterator<QString, QMap<QString, const QObject *> > iter(slotsMap);
    while (iter.hasNext()) {
        iter.next();
        const QString &key = iter.key();
        const QMap<QString, const QObject *> &slotMap = iter.value();

        SupportMethod supportMethod = (SupportMethod)key.toInt();
        const QObject *receiver = slotMap.first();
        QString receiverSlot = slotMap.firstKey();
        QString slot;
        QStringList slotTypes;

        extractSlot(receiverSlot, slot, slotTypes);


        if (supportMethod == AutoInfer) {


//            if (slotTypes.contains(TYPE_TO_STRING(QByteArray))) {
//                connect(this, SIGNAL(finished(QByteArray)), receiver, receiverSlot.toStdString().data());
//            }
//            else if (slotTypes.contains(TYPE_TO_STRING(QVariantMap))) {
//                connect(this, SIGNAL(finished(QVariantMap)), receiver, receiverSlot.toStdString().data());
//            }
//            else if (slotTypes.contains(TYPE_TO_STRING(QNetworkReply*))) {
//                connect(this, SIGNAL(finished(QNetworkReply*)), receiver, receiverSlot.toStdString().data());
//            }
//            else if (slotTypes == QStringList({TYPE_TO_STRING(qint64), TYPE_TO_STRING(qint64)})) {
//                connect(this, SIGNAL(downloadProgress(qint64, qint64)), receiver, receiverSlot.toStdString().data());
//            }
//            else if (slotTypes == QStringList({TYPE_TO_STRING(QNetworkReply::NetworkError)})) {
//                connect(this, SIGNAL(error(QNetworkReply::NetworkError)), receiver, receiverSlot.toStdString().data());
//            }
//            else if (slotTypes == QStringList({TYPE_TO_STRING(QString)})) {
//                connect(this, SIGNAL(error(QString)), receiver, receiverSlot.toStdString().data());
//            }
//            else {
//                qDebug()<<"Don't support type: "<<slotTypes;
//                qDebug()<<"Support Type: "<<supportSlotTypeList;
//            }
        }
        else if (supportMethod == onResponse_QByteArray) {

        }
    }
}

HttpResponse::HttpResponse()
{

}
