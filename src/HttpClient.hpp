/**********************************************************
 * Author: Qt君
 * 微信公众号: Qt君(文章首发)
 * Website: qthub.com(后续更新)
 * Email:  2088201923@qq.com
 * QQ交流群: 732271126
 * LISCENSE: MIT
**********************************************************/
#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonObject>
#include <QJsonDocument>

#include <QMetaEnum>
#include <QUrlQuery>
#include <QBuffer>

#include <QTimer>
#include <QRegExp>
#include <QEventLoop>
#include <QDebug>

#include <functional>

namespace AeaQt {

class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    friend class HttpRequest;

    inline HttpClient();
    inline ~HttpClient();

    inline HttpRequest get(const QString &url);
    inline HttpRequest post(const QString &url);
    inline HttpRequest put(const QString &url);

    inline HttpRequest send(const QString &url, Operation op = GetOperation);
};

class HttpResponse : public QObject
{
    Q_OBJECT
public:
    /*
     * Support Reflex Method
     * default: AutoInfer
     * AutoInfer: Automatic derivation based on type
     */
    enum SupportMethod {
        Invalid = 0,
        AutoInfer,
        onResponse_QNetworkReply_A_Pointer,    /* method: void function(QNetworkReply* reply); Is_AutoInfer: true */
        onResponse_QByteArray,                 /* method: void function(QByteArray data); Is_AutoInfer: true */
        onResponse_QVariantMap,                /* method: void function(QVariantMap map); Is_AutoInfer: true */
        onDownloadProgress_qint64_qint64,      /* method: void function(qint64 bytesReceived, qint64 bytesTotal); Is_AutoInfer: true */
        onError_QNetworkReply_To_NetworkError, /* method: void function(QNetworkReply::NetworkError error); Is_AutoInfer: true */
        onError_QString,                       /* method: void function(QString errorString); Is_AutoInfer: true */
        onError_QNetworkReply_To_NetworkError_QNetworkReply_A_Pointer, /* method: void function(QNetworkReply::NetworkError error, QNetworkReply* reply); Is_AutoInfer: true */
        onError_QString_QNetworkReply_A_Poniter/* method: void function(QString errorString, QNetworkReply* reply); Is_AutoInfer: true */
    };

    inline explicit HttpResponse(QNetworkReply *networkReply,
                          const QMultiMap<SupportMethod, QPair<QString, QVariant> > &slotsMap,
                          const int &timeout,
                          bool isBlock);

    inline virtual ~HttpResponse();

    inline QNetworkReply *networkReply();

signals:
    void finished(QNetworkReply *reply);
    void finished(QByteArray data);
    void finished(QVariantMap map);
    void error(QString errorString);
    void error(QNetworkReply::NetworkError error);
    void error(QString errorString, QNetworkReply *reply);
    void error(QNetworkReply::NetworkError error, QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    inline void slotsMapOperation(QMultiMap<SupportMethod, QPair<QString, QVariant> > &slotsMap);

private slots:
    inline void onFinished();
    inline void onError(QNetworkReply::NetworkError error);
    inline void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    inline HttpResponse();

private:
   QMultiMap<SupportMethod, QPair<QString, QVariant> > m_slotsMap;
   QNetworkReply *m_networkReply;
};

class HttpRequest
{
public:
    enum BodyType {
        None = 0, // This request does not have a body.
        X_Www_Form_Urlencoded, // x-www-form-urlencoded
        Raw_Text_Json, // application/json
    };

    inline explicit HttpRequest(QNetworkAccessManager::Operation op, HttpClient *jsonHttpClient);
    inline virtual ~HttpRequest();

    inline HttpRequest &url(const QString &url);
    inline HttpRequest &header(const QString &key, const QVariant &value);
    inline HttpRequest &headers(const QMap<QString, QVariant> &headers);

    inline HttpRequest &queryParam(const QString &key, const QVariant &value);
    inline HttpRequest &queryParams(const QMap<QString, QVariant> &params);

    /* Mainly used for identification */
    inline HttpRequest &userAttribute(const QVariant &value);

    inline HttpRequest &body(const QVariantMap &content);
    inline HttpRequest &body(const QJsonObject &content);
    inline HttpRequest &body(const QByteArray &content);

    /*
     * @onRespone slot support type: void function(QVariantMap resultMap) OR
     *                               void function(QByteArray resultData) OR
     *                               void function(QNetworkReply* reply)
     * note: The same type is only triggered once
     */
    inline HttpRequest &onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type = HttpResponse::AutoInfer);
    inline HttpRequest &onResponse(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onResponse(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onResponse(std::function<void (QByteArray)> lambda);
    inline HttpRequest &onResponse(std::function<void (qint64, qint64)> lambda);
    /*
     * @onError slot support type: void function(QNetworkReply::NetworkError error)
     *                             void function(QString errorString);
     *                             void function(QNetworkReply::NetworkError error, QNetworkReply* reply);
     *                             void function(QString errorString, QNetworkReply* reply);
     * note: The same type is only triggered once
     */
    inline HttpRequest &onError(const QObject *receiver, const char *slot);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onError(std::function<void (QString)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError, QNetworkReply*)> lambda);
    inline HttpRequest &onError(std::function<void (QString, QNetworkReply*)> lambda);

    /**
     * @brief msec <= 0, disable timeout
     *        msec >  0, enable timeout
     */
    inline HttpRequest &timeout(const int &msec = -1);

    /**
     * @brief Block current thread, entering an event loop.
     */
    inline HttpRequest &block();

    inline HttpResponse *exec();

private:
    inline HttpRequest();
    inline HttpRequest &onResponse(QVariant lambda);

private:
    QNetworkRequest                  m_networkRequest;
    QByteArray                       m_body;
    QNetworkAccessManager::Operation m_op;
    HttpClient                      *m_httpService;
    int                              m_timeout;
    bool                             m_isBlock;
    QMultiMap<HttpResponse::SupportMethod, QPair<QString, QVariant>> m_slotsMap;
};

class HttpResponseTimeout : public QObject {
    Q_OBJECT
public:
    HttpResponseTimeout(QNetworkReply *parent = NULL, const int timeout = -1) : QObject(parent) {
        if (timeout > 0)
            QTimer::singleShot(timeout, this, SLOT(onTimeout()));
    }

private slots:
    void onTimeout() {
        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            reply->abort();
            reply->deleteLater();
        }
    }
};

#ifdef QT_APP_DEBUG
#define _debugger qDebug().noquote().nospace() \
                          << "[AeaQt::Network] Debug: -> " \
                          << "function: " << __func__ << "; " \
                          << "line: " << __LINE__ << "; "
#else
#define _debugger QNoDebug()
#endif

#define _warning qWarning().noquote().nospace() \
                           << "[AeaQt::Network] Warning: -> " \
                           << "function: " << __func__ << "; " \
                           << "line: " << __LINE__ << "; "

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

HttpRequest::HttpRequest()
{
    m_isBlock = false;
}

HttpRequest::~HttpRequest()
{
}

HttpRequest::HttpRequest(QNetworkAccessManager::Operation op, HttpClient *jsonHttpClient) :
    m_body(QByteArray()),
    m_op(op),
    m_httpService(jsonHttpClient),
    m_isBlock(false),
    m_timeout(-1)
{
}

HttpRequest &HttpRequest::url(const QString &url)
{
    m_networkRequest.setUrl(QUrl(url));
    return *this;
}

HttpRequest &HttpRequest::header(const QString &key, const QVariant &value)
{
    m_networkRequest.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toString().toStdString().data()));

    return *this;
}

HttpRequest &HttpRequest::headers(const QMap<QString, QVariant> &headers)
{
   QMapIterator<QString, QVariant> iter(headers);
   while (iter.hasNext()) {
       iter.next();
       header(iter.key(), iter.value());
   }

   return *this;
}

HttpRequest &HttpRequest::body(const QVariantMap &content)
{
    m_body = QJsonDocument(QJsonObject::fromVariantMap(content)).toJson();
    return *this;
}

HttpRequest &HttpRequest::body(const QJsonObject &content)
{
    m_body = QJsonDocument(QJsonObject::fromVariantMap(content.toVariantMap())).toJson();
    return *this;
}

HttpRequest &HttpRequest::body(const QByteArray &content)
{
    m_body = content;
    return *this;
}

#if 0
HttpRequest &HttpRequest::body(const QVariant &body)
{
    /// clear m_jsonBody
    m_jsonBody = QJsonObject();

    if (type == X_Www_Form_Urlencoded) {
        QUrl url;
        QUrlQuery urlQuery(url);

        if (body.type() == QVariant::Map
            || body.typeName() ==  QMetaType::typeName(QMetaType::QJsonObject)) {

            QMapIterator<QString, QVariant> i(body.toMap());
            while (i.hasNext()) {
                i.next();
                urlQuery.addQueryItem(i.key(), i.value().toString());
            }

            url.setQuery(urlQuery);
            m_body = url.toString(QUrl::FullyEncoded).toUtf8().remove(0, 1);
        }
        else {
            m_body = body.toByteArray();
        }
    }
    else if (type == Raw_Text_Json) {
        if (body.type() == QVariant::Map
            || body.typeName() ==  QMetaType::typeName(QMetaType::QJsonObject)) {

            m_body = QJsonDocument(QJsonObject::fromVariantMap(body.toMap())).toJson();
        }
        else {
            _warning << "This is not data in JSON format(QVariantMap or QJsonObject).";
            m_body = QByteArray();
            // warning output
        }
    }
    else {
        m_body = QByteArray();
        _warning << "Disable body.";
    }

    _debugger << "Body Content:" << m_body;
    return *this;
}
#endif

HttpRequest &HttpRequest::onResponse(const QObject *receiver, const char *slot, HttpResponse::SupportMethod type)
{
    m_slotsMap.insert(type, {slot, QVariant::fromValue((QObject *)receiver)});
    return *this;
}

HttpRequest &HttpRequest::onResponse(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QVariantMap)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QByteArray)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(const QObject *receiver, const char *slot)
{
    return onResponse(receiver, slot, HttpResponse::AutoInfer);
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QString)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError, QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QString, QNetworkReply *)> lambda)
{
    return onResponse(QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::timeout(const int &msec)
{
    m_timeout = msec;
    return *this;
}

HttpRequest &HttpRequest::block()
{
    m_isBlock = true;
    return *this;
}

HttpRequest &HttpRequest::onResponse(QVariant lambda)
{
    m_slotsMap.insert(HttpResponse::AutoInfer, {lambda.typeName(), lambda});

    return *this;
}

HttpResponse *HttpRequest::exec()
{
    static const char *s_httpOperation[] = {
        "UnknownOperation",
        "HeadOperation",
        "GetOperation",
        "PutOperation",
        "PostOperation",
        "DeleteOperation",
        "CustomOperation"
    };

    QNetworkReply* reply = NULL;
    QBuffer* sendBuffer = new QBuffer();
    if (! m_body.isEmpty()) {
        sendBuffer->setData(m_body);
    }

    _debugger << "Http Client info: ";
    _debugger << "Type: " << s_httpOperation[m_op];
    _debugger << "Url: " << m_networkRequest.url().toString();
    QString headers;
    for (int i = 0; i < m_networkRequest.rawHeaderList().count(); i++) {
        QString each = m_networkRequest.rawHeaderList().at(i);
        QString header = m_networkRequest.rawHeader(each.toUtf8());
        headers += QString("%1: %2;").arg(each)
                                     .arg(header);
    }
    _debugger << "Header: " << headers;
    _debugger << "Send buffer(Body):\r\n" << m_body;

    reply = m_httpService->createRequest(m_op, m_networkRequest, sendBuffer);

    if (reply == NULL) {
        sendBuffer->deleteLater();
        return NULL;
    }
    else {
        sendBuffer->setParent(reply);
    }

    return new HttpResponse(reply, m_slotsMap, m_timeout, m_isBlock);
}

HttpRequest &HttpRequest::queryParam(const QString &key, const QVariant &value)
{
    QUrl url(m_networkRequest.url());
    QUrlQuery urlQuery(url);

    urlQuery.addQueryItem(key, value.toString());
    url.setQuery(urlQuery);

    m_networkRequest.setUrl(url);

    return *this;
}

HttpRequest &HttpRequest::queryParams(const QMap<QString, QVariant> &params)
{
    QMapIterator<QString, QVariant> iter(params);
    while (iter.hasNext()) {
        iter.next();
        queryParam(iter.key(), iter.value());
    }

    return *this;
}

HttpRequest &HttpRequest::userAttribute(const QVariant &value)
{
    m_networkRequest.setAttribute(QNetworkRequest::User, value);
    return *this;
}



HttpClient::HttpClient()
{
}

HttpClient::~HttpClient()
{
}

HttpRequest HttpClient::get(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::GetOperation, this).url(url);
}

HttpRequest HttpClient::post(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::PostOperation, this).url(url);
}

HttpRequest HttpClient::put(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::PutOperation, this).url(url);
}

HttpRequest HttpClient::send(const QString &url, QNetworkAccessManager::Operation op)
{
    return HttpRequest(op, this).url(url);
}

}

Q_DECLARE_METATYPE(std::function<void (QNetworkReply*)>)
Q_DECLARE_METATYPE(std::function<void (QByteArray)>)
Q_DECLARE_METATYPE(std::function<void (QVariantMap)>)

Q_DECLARE_METATYPE(std::function<void (QString)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError, QNetworkReply *)>)
Q_DECLARE_METATYPE(std::function<void (QString, QNetworkReply *)>)

Q_DECLARE_METATYPE(std::function<void (qint64, qint64)>)
#endif // HTTPCLIENT_HPP
