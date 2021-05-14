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

#include <utility>
#include <functional>


namespace AeaQt {

enum HandleType {
    h_onFinished = 0,
    h_onError,
    h_onDownloadProgress
};

class HttpClient;
class HttpRequest;
class HttpResponse;

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

    // onFinished == onSuccess
    inline HttpRequest &onFinished(const QObject *receiver, const char *slot);
    inline HttpRequest &onFinished(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onFinished(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onFinished(std::function<void (QByteArray)> lambda);

    inline HttpRequest &onDownloadProgress(const QObject *receiver, const char *slot);
    inline HttpRequest &onDownloadProgress(std::function<void (qint64, qint64)> lambda);

    // onError == onFailed
    inline HttpRequest &onError(const QObject *receiver, const char *slot);
    inline HttpRequest &onError(std::function<void (QString)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply*)> lambda);

    // onFinished == onSuccess
    inline HttpRequest &onSuccess(const QObject *receiver, const char *slot);
    inline HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);

    // onError == onFailed
    inline HttpRequest &onFailed(const QObject *receiver, const char *slot);
    inline HttpRequest &onFailed(std::function<void (QString)> lambda);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply*)> lambda);

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
    inline HttpRequest &onResponse(HandleType type, QVariant lambda);

private:
    QNetworkRequest                  m_networkRequest;
    QByteArray                       m_body;
    QNetworkAccessManager::Operation m_op;
    HttpClient                      *m_httpService;
    int                              m_timeout;
    bool                             m_isBlock;
    QMap<HandleType, QPair<QString, QVariant> > m_handleMap;
};

class HttpResponse : public QObject
{
    Q_OBJECT
public:
    inline explicit HttpResponse(QNetworkReply *reply,
                          const QMap<HandleType, QPair<QString, QVariant> > &m_handleMap,
                          const int &timeout,
                          bool isBlock);

    inline virtual ~HttpResponse() {}

    QNetworkReply *reply() { return static_cast<QNetworkReply*>(this->parent()); }

signals:
    void finished(QNetworkReply *reply);
    void finished(QString result);
    void finished(QByteArray result);
    void finished(QVariantMap resultMap);

    void downloadProgress(qint64, qint64);

    void error(QByteArray error);
    void error(QString errorString);
    void error(QNetworkReply::NetworkError error);
    void error(QNetworkReply *reply);

private slots:
    inline void onFinished();
    inline void onError(QNetworkReply::NetworkError error);
    inline void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
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

#define T2S(t) (QString(#t).remove(QRegExp("\\s"))) //type to string

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

HttpRequest &HttpRequest::onFinished(const QObject *receiver, const char *slot)
{
    m_handleMap.insert(h_onFinished, {slot, QVariant::fromValue((QObject *)receiver)});
    return *this;
}

HttpRequest &HttpRequest::onFinished(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onFinished(std::function<void (QVariantMap)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onFinished(std::function<void (QByteArray)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onDownloadProgress(const QObject *receiver, const char *slot)
{
    m_handleMap.insert(h_onDownloadProgress, {slot, QVariant::fromValue((QObject *)receiver)});
    return *this;
}

HttpRequest &HttpRequest::onDownloadProgress(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(h_onDownloadProgress, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(const QObject *receiver, const char *slot)
{
    m_handleMap.insert(h_onError, {slot, QVariant::fromValue((QObject *)receiver)});
    return *this;
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError)> lambda)
{
    return onResponse(h_onError, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QString)> lambda)
{
    return onResponse(h_onError, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(h_onError, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onSuccess(const QObject *receiver, const char *slot)
{
    return onFinished(receiver, slot);
}

HttpRequest &HttpRequest::onSuccess(std::function<void (QNetworkReply *)> lambda)
{
    return onFinished(lambda);
}

HttpRequest &HttpRequest::onSuccess(std::function<void (QVariantMap)> lambda)
{
    return onFinished(lambda);
}

HttpRequest &HttpRequest::onSuccess(std::function<void (QByteArray)> lambda)
{
    return onFinished(lambda);
}

HttpRequest &HttpRequest::onFailed(const QObject *receiver, const char *slot)
{
    return onError(receiver, slot);
}

HttpRequest &HttpRequest::onFailed(std::function<void (QNetworkReply::NetworkError)> lambda)
{
    return onError(lambda);
}

HttpRequest &HttpRequest::onFailed(std::function<void (QString)> lambda)
{
    return onError(lambda);
}

HttpRequest &HttpRequest::onFailed(std::function<void (QNetworkReply *)> lambda)
{
    return onError(lambda);
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

HttpRequest &HttpRequest::onResponse(HandleType type, QVariant lambda)
{
    m_handleMap.insert(type, {lambda.typeName(), lambda});

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

    return new HttpResponse(reply, m_handleMap, m_timeout, m_isBlock);
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

HttpResponse::HttpResponse(QNetworkReply *reply,
                           const QMap<HandleType, QPair<QString, QVariant> > &m_handleMap,
                           const int &timeout,
                           bool isBlock) : QObject(reply)
{
    new HttpResponseTimeout(reply, timeout);

    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));

    for (auto each : m_handleMap.toStdMap()) {
        HandleType key                 = each.first;
        QPair<QString, QVariant> value = each.second;

        QString lambdaString = value.first;
        QVariant lambda      = value.second;
        const QObject *receiver = lambda.value<QObject *>();
        char method[512] = {0};
        strncpy(method, lambdaString.toStdString().data(), 512);

        if (key == h_onFinished) {

            if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else if (lambdaString == T2S(std::function<void (QByteArray)>)) {
                connect(this,
                        QOverload<QByteArray>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QByteArray)>>());
            }
            else {
                QList<const char *> signalsList = {
                    SIGNAL(finished(QNetworkReply *)),
                    SIGNAL(finished(QByteArray)),
                    SIGNAL(finished(QString)),
                    SIGNAL(finished(QVariantMap))};

                bool isConnected = false;
                for (auto signal : signalsList) {
                    if (QMetaObject::checkConnectArgs(signal, method)) {
                        isConnected = true;

                        connect(this, signal, receiver, method);
                    }

                }

                if (!isConnected) {
                    qDebug() << lambdaString.remove(0, 1) << "failed!";
                }
            }
        }
        else if (key == h_onDownloadProgress) {
            if (lambdaString == T2S(std::function<void (qint64, qint64)>)) {
                connect(this,
                        QOverload<qint64, qint64>::of(&HttpResponse::downloadProgress),
                        lambda.value<std::function<void (qint64, qint64)>>());
            }
            else {
                // fixme
                QList<const char *> signalsList = {
                    SIGNAL(downloadProgress(qint64, qint64))
                };

                bool isConnected = false;
                for (auto signal : signalsList) {
                    if (QMetaObject::checkConnectArgs(signal, method)) {
                        isConnected = true;

                        connect(this, signal, receiver, method);
                    }

                }

                if (!isConnected) {
                    qDebug() << lambdaString.remove(0, 1) << "failed!";
                }
            }

        }
        else if (key == h_onError) {
            if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else if (lambdaString == T2S(std::function<void (QString)>)) {
                connect(this,
                        QOverload<QString>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QString)>>());
            }
            else if (lambdaString == T2S(std::function<void (QNetworkReply::NetworkError)>)) {
                connect(this,
                        QOverload<QNetworkReply::NetworkError>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QNetworkReply::NetworkError)>>());
            }
            else {
                // fixme
                QList<const char *> signalsList = {
                    SIGNAL(error(QNetworkReply *)),
                    SIGNAL(error(QString)),
                    SIGNAL(error(QByteArray)),
                    SIGNAL(error(QNetworkReply::NetworkError))};

                bool isConnected = false;
                for (auto signal : signalsList) {
                    if (QMetaObject::checkConnectArgs(signal, method)) {
                        isConnected = true;

                        connect(this, signal, receiver, method);
                    }
                    else {
                        // do nothing
                    }
                }

                if (!isConnected) {
                    qDebug() << lambdaString.remove(0, 1) << "failed!";
                }
            }
        }
    }

    if (isBlock) {
        QEventLoop loop;
        QObject::connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

void HttpResponse::onFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());
    if (reply->error() != QNetworkReply::NoError)
        return;

    if (this->receivers(SIGNAL(finished(QNetworkReply*))) > 0) {
        emit finished(reply);
        return;
    }

    QByteArray result = reply->readAll();
    emit finished(result);

    emit finished(QString(result));

    QVariantMap resultMap = QJsonDocument::fromJson(result).object().toVariantMap();
    emit finished(resultMap);

    reply->deleteLater();
}

void HttpResponse::onError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());

    const QMetaObject & metaObject = QNetworkReply::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NetworkError"));
    QString errorString = reply->errorString().isEmpty() ? metaEnum.valueToKey(error) : reply->errorString();

    if (this->receivers(SIGNAL(error(QNetworkReply*))) > 0) {
        emit this->error(reply);
        return;
    }

    emit this->error(errorString);
    emit this->error(error);
    emit this->error(errorString.toLocal8Bit());

    reply->deleteLater();
}

void HttpResponse::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit this->downloadProgress(bytesReceived, bytesTotal);
}

}

Q_DECLARE_METATYPE(std::function<void (QByteArray)>)
Q_DECLARE_METATYPE(std::function<void (QString)>)
Q_DECLARE_METATYPE(std::function<void (QVariantMap)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply*)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError)>)
Q_DECLARE_METATYPE(std::function<void (qint64, qint64)>)

#endif // HTTPCLIENT_HPP
