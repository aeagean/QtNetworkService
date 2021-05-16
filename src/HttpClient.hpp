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
#include <QHttpMultiPart>

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
    h_onDownloadProgress,
    h_onTimeout
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

    HttpClient &instance() {
        static HttpClient client;
        return client;
    }

    inline HttpRequest get(const QString &url);
    inline HttpRequest post(const QString &url);
    inline HttpRequest put(const QString &url);

    inline HttpRequest send(const QString &url, Operation op = GetOperation);
};

class HttpRequest
{
public:
    inline explicit HttpRequest(QNetworkAccessManager::Operation op, HttpClient *jsonHttpClient);
    inline virtual ~HttpRequest();

    inline HttpRequest &url(const QString &url);
    inline HttpRequest &header(const QString &key, const QVariant &value);
    inline HttpRequest &headers(const QMap<QString, QVariant> &headers);

    inline HttpRequest &queryParam(const QString &key, const QVariant &value);
    inline HttpRequest &queryParams(const QMap<QString, QVariant> &params);

    /* Mainly used for identification */
    inline HttpRequest &userAttribute(const QVariant &value);

    inline HttpRequest &body(const QByteArray &raw);
    inline HttpRequest &bodyWithRaw(const QByteArray &raw);

    inline HttpRequest &body(const QJsonObject &json);
    inline HttpRequest &bodyWithJson(const QJsonObject &json);

    inline HttpRequest &body(const QVariantMap &formUrlencodedMap);
    inline HttpRequest &bodyWithFormUrlencoded(const QVariantMap &keyValueMap);

    // todo
    inline HttpRequest &body(QHttpMultiPart *multiPart);
    inline HttpRequest &bodyWithFile(const QString &file);
    inline HttpRequest &bodyWithFile(const QStringList &files);
    inline HttpRequest &bodyWithFile(const QMap<QString, QString> &fileMap); // => QMap<name, file>; like: { "car": "/home/example/car.jpeg" }

    // onFinished == onSuccess
    inline HttpRequest &onFinished(const QObject *receiver, const char *methoc);
    inline HttpRequest &onFinished(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onFinished(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onFinished(std::function<void (QByteArray)> lambda);

    inline HttpRequest &onDownloadProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadProgress(std::function<void (qint64, qint64)> lambda);

    // onError == onFailed
    inline HttpRequest &onError(const QObject *receiver, const char *method);
    inline HttpRequest &onError(std::function<void (QString)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply*)> lambda);

    // onFinished == onSuccess
    inline HttpRequest &onSuccess(const QObject *receiver, const char *method);
    inline HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);

    // onError == onFailed
    inline HttpRequest &onFailed(const QObject *receiver, const char *method);
    inline HttpRequest &onFailed(std::function<void (QString)> lambda);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply*)> lambda);

    /**
     * @brief msec <= 0, disable timeout
     *        msec >  0, enable timeout
     */
    inline HttpRequest &timeout(const int &msec = -1);
    inline HttpRequest &onTimeout(const QObject *receiver, const char *method);
    inline HttpRequest &onTimeout(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onTimeout(std::function<void ()> lambda);

    // do nothing. todo
    inline HttpRequest &retry(int count);

    // do nothing. todo
    inline HttpRequest &repeat(int count);

    /**
     * @brief Block current thread, entering an event loop.
     */
    inline HttpRequest &block();

    inline HttpResponse *exec();

    struct Params {
        enum BodyType {
            None = 0, // This request does not have a body.
            Raw,
            Raw_Json, // application/json
            X_Www_Form_Urlencoded, // x-www-form-urlencoded
            MultiPart
        };

        QNetworkAccessManager::Operation op;
        QNetworkRequest                  request;
        QNetworkReply                   *reply;
        HttpClient                      *httpClient;
        QPair<BodyType, QVariant>        body;
        int                              timeout; // ms
        bool                             isBlock;
        int                              retry;
        QMap<HandleType, QPair<QString, QVariant> > handleMap;

        Params()
        {
            reply = NULL;
            httpClient = NULL;
            isBlock = (false);
            retry = (0);
            timeout = (-1);
        }
    };

private:
    inline HttpRequest() = delete;
    inline HttpRequest &onResponse(HandleType type, const QObject *receiver, const char *method);
    inline HttpRequest &onResponse(HandleType type, QVariant lambda);

private:
    Params m_params;
};

class HttpResponse : public QObject
{
    Q_OBJECT
public:
    inline explicit HttpResponse(HttpRequest::Params params);
    inline virtual ~HttpResponse();

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

    void timeout();
    void timeout(QNetworkReply *reply);

private slots:
    inline void onFinished();
    inline void onError(QNetworkReply::NetworkError error);
    inline void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    inline void onTimeout();
};

class HttpResponseTimeout : public QObject {
    Q_OBJECT
public:
    HttpResponseTimeout(HttpResponse *parent, const int timeout = -1) : QObject(parent) {
        if (timeout > 0)
            QTimer::singleShot(timeout, parent, SLOT(onTimeout()));
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

HttpRequest::~HttpRequest()
{
}

HttpRequest::HttpRequest(QNetworkAccessManager::Operation op, HttpClient *httpClient)
{
    m_params.op = op;
    m_params.httpClient = httpClient;
}

HttpRequest &HttpRequest::url(const QString &url)
{
    m_params.request.setUrl(QUrl(url));
    return *this;
}

HttpRequest &HttpRequest::header(const QString &key, const QVariant &value)
{
    m_params.request.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toString().toStdString().data()));
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
    QMapIterator<QString, QVariant> i(content);

    QUrl url;
    QUrlQuery urlQuery(url);
    while (i.hasNext()) {
        i.next();
        urlQuery.addQueryItem(i.key(), i.value().toString());
    }

    url.setQuery(urlQuery);
    const QString &value = url.toString(QUrl::FullyEncoded).toUtf8().remove(0, 1);

    m_params.body = qMakePair(Params::X_Www_Form_Urlencoded, value);
    m_params.request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    return *this;
}

HttpRequest &HttpRequest::bodyWithFormUrlencoded(const QVariantMap &keyValueMap)
{
    return body(keyValueMap);
}

HttpRequest &HttpRequest::body(const QJsonObject &content)
{
    const QByteArray &value = QJsonDocument(content).toJson();
    m_params.body = qMakePair(Params::Raw_Json, value);
    m_params.request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    return *this;
}

HttpRequest &HttpRequest::bodyWithJson(const QJsonObject &json)
{
    return body(json);
}

HttpRequest &HttpRequest::body(const QByteArray &content)
{
    m_params.body = qMakePair(Params::Raw, content);
    return *this;
}

HttpRequest &HttpRequest::bodyWithRaw(const QByteArray &raw)
{
    return body(raw);
}

HttpRequest &HttpRequest::body(QHttpMultiPart *multiPart)
{
    m_params.body = qMakePair(Params::MultiPart, QVariant::fromValue(multiPart));
    return *this;
}

HttpRequest &HttpRequest::onFinished(const QObject *receiver, const char *method)
{
    return onResponse(h_onFinished, receiver, method);
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

HttpRequest &HttpRequest::onDownloadProgress(const QObject *receiver, const char *method)
{
    return onResponse(h_onDownloadProgress, receiver, method);
}

HttpRequest &HttpRequest::onDownloadProgress(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(h_onDownloadProgress, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onError(const QObject *receiver, const char *method)
{
    return onResponse(h_onError, receiver, method);
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

HttpRequest &HttpRequest::onSuccess(const QObject *receiver, const char *method)
{
    return onFinished(receiver, method);
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

HttpRequest &HttpRequest::onFailed(const QObject *receiver, const char *method)
{
    return onError(receiver, method);
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
    m_params.timeout = msec;
    return *this;
}

HttpRequest &HttpRequest::onTimeout(const QObject *receiver, const char *method)
{
    return onResponse(h_onTimeout, receiver, method);
}

HttpRequest &HttpRequest::onTimeout(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(h_onTimeout, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onTimeout(std::function<void ()> lambda)
{
    return onResponse(h_onTimeout, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::retry(int count)
{
    m_params.retry = count;
    return *this;
}

HttpRequest &HttpRequest::block()
{
    m_params.isBlock = true;
    return *this;
}

HttpRequest &HttpRequest::onResponse(HandleType type, QVariant lambda)
{
    m_params.handleMap.insert(type, {lambda.typeName(), lambda});
    return *this;
}

HttpRequest &HttpRequest::onResponse(HandleType type, const QObject *receiver, const char *method)
{
    m_params.handleMap.insert(type, {QMetaObject::normalizedSignature(method), QVariant::fromValue((QObject *)receiver)});
    return *this;
}

inline QDebug operator<<(QDebug &debug, const QNetworkAccessManager::Operation &op)
{
    switch (op) {
    case QNetworkAccessManager::UnknownOperation:
        debug  << "UnknownOperation";
        break;
    case QNetworkAccessManager::HeadOperation:
        debug  << "HeadOperation";
        break;
    case QNetworkAccessManager::GetOperation:
        debug  << "GetOperation";
        break;
    case QNetworkAccessManager::PostOperation:
        debug  << "PostOperation";
        break;
    case QNetworkAccessManager::PutOperation:
        debug  << "PutOperation";
        break;
    case QNetworkAccessManager::DeleteOperation:
        debug  << "DeleteOperation";
        break;
    case QNetworkAccessManager::CustomOperation:
        debug  << "CustomOperation";
        break;
    default:
        break;
    }

    return debug;
}

HttpResponse *HttpRequest::exec()
{
#ifdef QT_APP_DEBUG
    _debugger << "Http Client info: ";
    _debugger << "Url: " << m_params.request.url().toString();
    _debugger << "Type: " << m_params.op;
    QString headers;
    for (int i = 0; i < m_params.request.rawHeaderList().count(); i++) {
        QString each = m_params.request.rawHeaderList().at(i);
        QString header = m_params.request.rawHeader(each.toUtf8());
        headers += QString("%1: %2;").arg(each)
                                     .arg(header);
    }

    _debugger << "Header: " << headers;
    _debugger << "Body:\r\n" << m_params.body;
#endif

    static QMap<QNetworkAccessManager::Operation, QByteArray> verbMap = {
        {QNetworkAccessManager::GetOperation, "GET"},
        {QNetworkAccessManager::PostOperation, "POST"},
    };

    if (!verbMap.contains(m_params.op)) {
        qWarning() << "Url: [" << m_params.request.url().toString() << "]" << m_params.op << "not support!";
        return NULL;
    }

    using BodyType = HttpRequest::Params;
    if (m_params.body.first == BodyType::MultiPart) {
        QHttpMultiPart *multiPart = (QHttpMultiPart *)(m_params.body.second.value<QHttpMultiPart*>());
        m_params.reply = m_params.httpClient->sendCustomRequest(m_params.request,
                                                                verbMap.value(m_params.op),
                                                                multiPart);
        multiPart->setParent(m_params.reply);

    }
    else {
        m_params.reply = m_params.httpClient->sendCustomRequest(m_params.request,
                                                                verbMap.value(m_params.op),
                                                                m_params.body.second.toByteArray());
    }

    if (m_params.reply == NULL) {
        qWarning() << "http reply invalid";
        return NULL;
    }

    return new HttpResponse(m_params);
}

HttpRequest &HttpRequest::queryParam(const QString &key, const QVariant &value)
{
    QUrl url(m_params.request.url());
    QUrlQuery urlQuery(url);

    urlQuery.addQueryItem(key, value.toString());
    url.setQuery(urlQuery);

    m_params.request.setUrl(url);

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
    m_params.request.setAttribute(QNetworkRequest::User, value);
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

HttpResponse::HttpResponse(HttpRequest::Params params) : QObject(params.reply)
{
    int timeout = params.timeout;
    QNetworkReply *reply = params.reply;
    auto handleMap = params.handleMap;
    int retryCount = params.retry;
    int isBlock = params.isBlock;

    new HttpResponseTimeout(this, timeout);

    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));

    auto func = [&](const QStringList &signalsList,
                    const QObject *receiver,
                    const QString &method)
    {
        bool isConnected = false;
        for (QString signal : signalsList) {
            signal = QMetaObject::normalizedSignature(qPrintable(signal));
            if (QMetaObject::checkConnectArgs(qPrintable(signal), qPrintable(method))) {
                isConnected = true;

                connect(this, qPrintable(signal), receiver, qPrintable(method));
            }
            else {
                // do nothing
            }
        }

        if (!isConnected) {
            _warning << QString(method).remove(0, 1) << "failed!";
        }
    };

    for (auto each : handleMap.toStdMap()) {
        HandleType key                 = each.first;
        QPair<QString, QVariant> value = each.second;

        QVariant lambda      = value.second;
        QString lambdaString = value.first;

        const QObject *receiver = lambda.value<QObject *>();
        QString method          = lambdaString;

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
                QStringList signalsList = {
                    SIGNAL(finished(QNetworkReply *)),
                    SIGNAL(finished(QByteArray)),
                    SIGNAL(finished(QString)),
                    SIGNAL(finished(QVariantMap))};

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onDownloadProgress) {
            if (lambdaString == T2S(std::function<void (qint64, qint64)>)) {
                connect(this,
                        QOverload<qint64, qint64>::of(&HttpResponse::downloadProgress),
                        lambda.value<std::function<void (qint64, qint64)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(downloadProgress(qint64, qint64))
                };

                func(signalsList, receiver, method);
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
                QStringList signalsList = {
                    SIGNAL(error(QNetworkReply *)),
                    SIGNAL(error(QString)),
                    SIGNAL(error(QByteArray)),
                    SIGNAL(error(QNetworkReply::NetworkError))};

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onTimeout) {
            if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::timeout),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::timeout),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(error(QNetworkReply *)),
                    SIGNAL(error())
                };

                func(signalsList, receiver, method);
            }
        }
        else {
            // do nothing
        }
    }

    if (isBlock) {
        QEventLoop loop;
        QObject::connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

HttpResponse::~HttpResponse()
{
    _debugger << "destory: " << __FUNCTION__;
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

void HttpResponse::onTimeout()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());
    if (reply->isRunning()) {
        reply->abort();

        if (this->receivers(SIGNAL(timeout(QNetworkReply*))) > 0) {
            emit this->timeout(reply);
            return;
        }

        emit this->timeout();

        reply->deleteLater();
    }
}

}

Q_DECLARE_METATYPE(std::function<void ()>)
Q_DECLARE_METATYPE(std::function<void (QByteArray)>)
Q_DECLARE_METATYPE(std::function<void (QString)>)
Q_DECLARE_METATYPE(std::function<void (QVariantMap)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply*)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError)>)
Q_DECLARE_METATYPE(std::function<void (qint64, qint64)>)

#endif // HTTPCLIENT_HPP
