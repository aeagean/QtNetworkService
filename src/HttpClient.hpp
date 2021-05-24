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
#include <QFile>
#include <QIODevice>
#include <QFileInfo>

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
    h_onUploadProgress,
    h_onTimeout,
    h_onReadyRead,
    h_onDownloadSuccess,
    h_onDownloadFailed,
    h_onEncrypted,
    h_onMetaDataChanged,
    h_onPreSharedKeyAuthenticationRequired,
    h_onRedirectAllowed,
    h_onRedirected,
    h_onSslErrors,
    h_onRetried,
    h_onRepeated
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
    inline HttpRequest &header(QNetworkRequest::KnownHeaders header, const QVariant &value);
    inline HttpRequest &headers(const QMap<QString, QVariant> &headers);
    inline HttpRequest &headers(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers);

    inline HttpRequest &queryParam(const QString &key, const QVariant &value);
    inline HttpRequest &queryParams(const QMap<QString, QVariant> &params);

    /* Mainly used for identification */
    inline HttpRequest &userAttribute(const QVariant &value);
    inline HttpRequest &attribute(QNetworkRequest::Attribute attribute, const QVariant &value);

    inline HttpRequest &body(const QByteArray &raw);
    inline HttpRequest &bodyWithRaw(const QByteArray &raw);

    inline HttpRequest &body(const QJsonObject &json);
    inline HttpRequest &bodyWithJson(const QJsonObject &json);

    inline HttpRequest &body(const QVariantMap &formUrlencodedMap);
    inline HttpRequest &bodyWithFormUrlencoded(const QVariantMap &keyValueMap);

    inline HttpRequest &body(QHttpMultiPart *multiPart);
    inline HttpRequest &bodyWithMultiPart(QHttpMultiPart *multiPart);

    // multi-paramws
    inline HttpRequest &body(const QString &key, const QString &file);
    inline HttpRequest &bodyWithFile(const QString &key, const QString &file);
    inline HttpRequest &bodyWithFile(const QMap<QString/*key*/, QString/*file*/> &fileMap); // => QMap<key, file>; like: { "key": "/home/example/car.jpeg" }

    inline HttpRequest &ignoreSslErrors(const QList<QSslError> &errors);
    inline HttpRequest &sslConfiguration(const QSslConfiguration &config);

    inline HttpRequest &Priority(QNetworkRequest::Priority priority);
    inline HttpRequest &MaximumRedirectsAllowed(int maxRedirectsAllowed);
    inline HttpRequest &originatingObject(QObject *object);
    inline HttpRequest &readBufferSize(qint64 size);

    // onFinished == onSuccess
    inline HttpRequest &onSuccess(const QObject *receiver, const char *method);
    inline HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);

    // onFinished == onSuccess
    inline HttpRequest &onFinished(const QObject *receiver, const char *method);
    inline HttpRequest &onFinished(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onFinished(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onFinished(std::function<void (QByteArray)> lambda);

    // onResponse == onSuccess. note: DEPRECATED
    inline HttpRequest &onResponse(const QObject *receiver, const char *method);
    inline HttpRequest &onResponse(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onResponse(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onResponse(std::function<void (QByteArray)> lambda);

    inline HttpRequest &onDownloadProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadProgress(std::function<void (qint64, qint64)> lambda);

    inline HttpRequest &onUploadProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onUploadProgress(std::function<void (qint64, qint64)> lambda);

    // onError == onFailed
    inline HttpRequest &onError(const QObject *receiver, const char *method);
    inline HttpRequest &onError(std::function<void (QString)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply*)> lambda);

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

    inline HttpRequest &download();
    inline HttpRequest &download(const QString &file);

    inline HttpRequest &onDownloadSuccess(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadSuccess(std::function<void ()> lambda);
    inline HttpRequest &onDownloadSuccess(std::function<void (QString)> lambda);

    inline HttpRequest &onDownloadFailed(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadFailed(std::function<void ()> lambda);
    inline HttpRequest &onDownloadFailed(std::function<void (QString)> lambda);

    inline HttpRequest &onReadyRead(const QObject *receiver, const char *method);
    inline HttpRequest &onReadyRead(std::function<void (QNetworkReply*)> lambda);

    inline HttpRequest &retry(int count);
    inline HttpRequest &onRetried(const QObject *receiver, const char *method);
    inline HttpRequest &onRetried(std::function<void ()> lambda);

    // [0] do nothing. todo
    inline HttpRequest &repeat(int count);
    inline HttpRequest &onRepeated(const QObject *receiver, const char *method);
    inline HttpRequest &onRepeated(std::function<void ()> lambda);
    // [0] do nothing. todo

    /**
     * @brief Block current thread, entering an event loop.
     */
    inline HttpRequest &block();
    // block => sync
    inline HttpRequest &sync();

    inline HttpResponse *exec();

    struct Params {
        enum BodyType {
            None = 0, // This request does not have a body.
            Raw,
            Raw_Json, // application/json
            X_Www_Form_Urlencoded, // x-www-form-urlencoded
            FileMap,
            MultiPart
        };

        enum DownloadEnabled {
            Disabled,
            Enabled,
        };

        QNetworkAccessManager::Operation op;
        QNetworkReply                   *reply;
        QNetworkRequest                  request;
        HttpClient                      *httpClient;
        QPair<BodyType, QVariant>        body;
        int                              timeout; // ms
        bool                             isBlock;
        int                              retryCount;
        bool                             enabledRetry;
        int                              repeatCount;

        QPair<DownloadEnabled, QString>  downloadFile;
        QMap<HandleType, QPair<QString, QVariant> > handleMap;
        QList<QSslError>  ignoreSslErrors;
        qint64            readBufferSize;

        Params()
        {
            reply = NULL;
            httpClient = NULL;
            isBlock = (false);
            retryCount = (0);
            repeatCount = 1;
            enabledRetry = false;
            timeout = (-1);
            body = qMakePair(BodyType::None, QByteArray());
            downloadFile = qMakePair(DownloadEnabled::Disabled, QString(""));
            readBufferSize = 0;
        }
    };

protected:
    inline HttpRequest &enabledRetry(bool isEnabled);

    friend class HttpResponse;

private:
    inline HttpRequest() = delete;
    inline HttpRequest &onResponse(HandleType type, const QObject *receiver, const char *method);
    inline HttpRequest &onResponse(HandleType type, QVariant lambda);

private:
    Params m_params;
    QMap<QString, QString> m_fileMap;
};

class HttpResponse : public QObject
{
    Q_OBJECT
public:
    inline explicit HttpResponse(HttpRequest::Params params, HttpRequest httpRequest);
    inline virtual ~HttpResponse();

    QNetworkReply *reply() { return static_cast<QNetworkReply*>(this->parent()); }

signals:
    void finished(QNetworkReply *reply);
    void finished(QString result);
    void finished(QByteArray result);
    void finished(QVariantMap resultMap);

    void downloadProgress(qint64, qint64);
    void uploadProgress(qint64, qint64);

    void error(QByteArray error);
    void error(QString errorString);
    void error(QNetworkReply::NetworkError error);
    void error(QNetworkReply *reply); 

    void timeout();
    void timeout(QNetworkReply *reply);

    void readyRead(QNetworkReply *reply);

    void downloadFinished();
    void downloadFinished(QString file);

    void downloadError();
    void downloadError(QString errorString);

    void encrypted();
    void metaDataChanged();

    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void redirectAllowed();
    void redirected(QUrl url);
    void sslErrors(QList<QSslError> errors);

    void retried();
    void repeated();

private slots:
    inline void onFinished();
    inline void onError(QNetworkReply::NetworkError error);
    inline void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    inline void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    inline void onTimeout();
    inline void onReadyRead();
    inline void onReadOnceReplyHeader();

    inline void onEncrypted();
    inline void onMetaDataChanged();
    inline void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    inline void onRedirectAllowed();
    inline void onRedirected(const QUrl &url);
    inline void onSslErrors(const QList<QSslError> &errors);

private:
    HttpRequest::Params m_params;
    HttpRequest         m_httpRequest;
    QFile               m_downloadFile;
    int                 m_retriesRemaining = 0;
    int                 m_repeated = 1;
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

HttpRequest &HttpRequest::header(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    m_params.request.setHeader(header, value);
    return *this;
}

HttpRequest &HttpRequest::headers(const QMap<QNetworkRequest::KnownHeaders, QVariant> &headers)
{
   QMapIterator<QNetworkRequest::KnownHeaders, QVariant> iter(headers);
   while (iter.hasNext()) {
       iter.next();
       header(iter.key(), iter.value());
   }

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

HttpRequest &HttpRequest::body(const QVariantMap &keyValueMap)
{
    return bodyWithFormUrlencoded(keyValueMap);
}

HttpRequest &HttpRequest::bodyWithFormUrlencoded(const QVariantMap &keyValueMap)
{
    QMapIterator<QString, QVariant> i(keyValueMap);

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

HttpRequest &HttpRequest::body(const QJsonObject &json)
{
    return bodyWithJson(json);
}

HttpRequest &HttpRequest::bodyWithJson(const QJsonObject &json)
{
    const QByteArray &value = QJsonDocument(json).toJson();
    m_params.body = qMakePair(Params::Raw_Json, value);
    m_params.request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    return *this;
}

HttpRequest &HttpRequest::body(const QByteArray &raw)
{
    return bodyWithRaw(raw);
}

HttpRequest &HttpRequest::bodyWithRaw(const QByteArray &raw)
{
    m_params.body = qMakePair(Params::Raw, raw);
    return *this;
}

HttpRequest &HttpRequest::body(QHttpMultiPart *multiPart)
{
    return bodyWithMultiPart(multiPart);
}

HttpRequest &HttpRequest::bodyWithMultiPart(QHttpMultiPart *multiPart)
{
    m_params.body = qMakePair(Params::MultiPart, QVariant::fromValue(multiPart));
    return *this;
}

HttpRequest &HttpRequest::download()
{
    return download("");
}

HttpRequest &HttpRequest::download(const QString &file)
{
    this->attribute(QNetworkRequest::FollowRedirectsAttribute, true);

    m_params.downloadFile = qMakePair(Params::DownloadEnabled::Enabled, file);
    return *this;
}

HttpRequest &HttpRequest::onDownloadSuccess(const QObject *receiver, const char *method)
{
    return onResponse(h_onDownloadSuccess, receiver, method);
}

HttpRequest &HttpRequest::onDownloadSuccess(std::function<void ()> lambda)
{
    return onResponse(h_onDownloadSuccess, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onDownloadSuccess(std::function<void (QString)> lambda)
{
    return onResponse(h_onDownloadSuccess, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onDownloadFailed(const QObject *receiver, const char *method)
{
    return onResponse(h_onDownloadFailed, receiver, method);
}

HttpRequest &HttpRequest::onDownloadFailed(std::function<void ()> lambda)
{
    return onResponse(h_onDownloadFailed, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onDownloadFailed(std::function<void (QString)> lambda)
{
    return onResponse(h_onDownloadFailed, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onReadyRead(const QObject *receiver, const char *method)
{
    return onResponse(h_onReadyRead, receiver, method);
}

HttpRequest &HttpRequest::onReadyRead(std::function<void (QNetworkReply *)> lambda)
{
    return onResponse(h_onReadyRead, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::body(const QString &key, const QString &file)
{
    return bodyWithFile(key, file);
}

HttpRequest &HttpRequest::bodyWithFile(const QString &key, const QString &filePath)
{
    QMap<QString, QString> map;
    map[key] = filePath;

    return bodyWithFile(map);
}

HttpRequest &HttpRequest::bodyWithFile(const QMap<QString, QString> &fileMap)
{
    auto &body = m_params.body;
    auto map = body.second.value<QMap<QString, QString>>();
    for (auto each : fileMap.toStdMap()) {
        map[each.first] = each.second;
    }

    body.first = Params::BodyType::FileMap;
    body.second = QVariant::fromValue(map);
    return *this;
}

HttpRequest &HttpRequest::ignoreSslErrors(const QList<QSslError> &errors)
{
    m_params.ignoreSslErrors = errors;
    return *this;
}

HttpRequest &HttpRequest::sslConfiguration(const QSslConfiguration &config)
{
    m_params.request.setSslConfiguration(config);
    return *this;
}

HttpRequest &HttpRequest::Priority(QNetworkRequest::Priority priority)
{
    m_params.request.setPriority(priority);
    return *this;
}

HttpRequest &HttpRequest::MaximumRedirectsAllowed(int maxRedirectsAllowed)
{
    m_params.request.setMaximumRedirectsAllowed(maxRedirectsAllowed);
    return *this;
}

HttpRequest &HttpRequest::originatingObject(QObject *object)
{
    m_params.request.setOriginatingObject(object);
    return *this;
}

HttpRequest &HttpRequest::readBufferSize(qint64 size)
{
    m_params.readBufferSize = size;
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

HttpRequest &HttpRequest::onUploadProgress(const QObject *receiver, const char *method)
{
    return onResponse(h_onUploadProgress, receiver, method);
}

HttpRequest &HttpRequest::onUploadProgress(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(h_onUploadProgress, QVariant::fromValue(lambda));
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
    m_params.retryCount = count;
    return *this;
}

HttpRequest &HttpRequest::onRetried(const QObject *receiver, const char *method)
{
    return onResponse(h_onRetried, receiver, method);
}

HttpRequest &HttpRequest::onRetried(std::function<void ()> lambda)
{
    return onResponse(h_onRetried, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::repeat(int count)
{
    m_params.repeatCount = count;
    return *this;
}

HttpRequest &HttpRequest::onRepeated(const QObject *receiver, const char *method)
{
    return onResponse(h_onRepeated, receiver, method);
}

HttpRequest &HttpRequest::onRepeated(std::function<void ()> lambda)
{
    return onResponse(h_onRepeated, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::block()
{
    m_params.isBlock = true;
    return *this;
}

HttpRequest &HttpRequest::sync()
{
    return block();
}

HttpRequest &HttpRequest::onResponse(const QObject *receiver, const char *method)
{
    return onResponse(h_onFinished, receiver, method);
}

HttpRequest &HttpRequest::onResponse(std::function<void (QNetworkReply*)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QVariantMap)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
}

HttpRequest &HttpRequest::onResponse(std::function<void (QByteArray)> lambda)
{
    return onResponse(h_onFinished, QVariant::fromValue(lambda));
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
        {QNetworkAccessManager::GetOperation,  "GET"},
        {QNetworkAccessManager::PostOperation, "POST"},
        {QNetworkAccessManager::PutOperation,  "PUT"},
    };

    if (!verbMap.contains(m_params.op)) {
        qWarning() << "Url: [" << m_params.request.url().toString() << "]" << m_params.op << "not support!";
        return NULL;
    }

    using BodyType = HttpRequest::Params::BodyType;
    BodyType bodyType = m_params.body.first;
    QVariant    &body = m_params.body.second;
    QNetworkRequest &request = m_params.request;
    HttpClient   *httpClient = m_params.httpClient;

    if (bodyType == BodyType::MultiPart) {
        QHttpMultiPart *multiPart = (QHttpMultiPart *)(body.value<QHttpMultiPart*>());
        QString contentType       = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());

        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

        m_params.reply = m_params.httpClient->sendCustomRequest(request,
                                                       verbMap.value(m_params.op),
                                                       multiPart);
        multiPart->setParent(m_params.reply);

    }
    else if (bodyType == BodyType::FileMap) {

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QString contentType = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());
        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

        const auto &fileMap = body.value<QMap<QString, QString>>();
        for (const auto &each : fileMap.toStdMap()) {
            const QString &key      = each.first;
            const QString &filePath = each.second;

            QFile *file = new QFile(filePath);
            file->open(QIODevice::ReadOnly);
            file->setParent(multiPart);

            // todo
            // part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));

            // note: "form-data; name=\"%1\";filename=\"%2\"" != "form-data; name=\"%1\";filename=\"%2\";"
            QString dispositionHeader = QString("form-data; name=\"%1\";filename=\"%2\"")
                    .arg(key)
                    .arg(QFileInfo(filePath).fileName());
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader, dispositionHeader);
            part.setBodyDevice(file);

            multiPart->append(part);
        }

        m_params.reply = httpClient->sendCustomRequest(request,
                                              verbMap.value(m_params.op),
                                              multiPart);
        multiPart->setParent(m_params.reply); // fixme if m_params.reply == NULL => multiPart memory leak
    }
    else {
        m_params.reply = m_params.httpClient->sendCustomRequest(request,
                                                                verbMap.value(m_params.op),
                                                                body.toByteArray());
    }

    if (m_params.reply == NULL) {
        qWarning() << "http reply invalid";
        return NULL;
    }

    // fixme
    m_params.reply->ignoreSslErrors(m_params.ignoreSslErrors);
    m_params.reply->setReadBufferSize(m_params.readBufferSize);

    return new HttpResponse(m_params, *this);
}

HttpRequest &HttpRequest::enabledRetry(bool isEnabled)
{
    m_params.enabledRetry = isEnabled;
    return *this;
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

HttpRequest &HttpRequest::attribute(QNetworkRequest::Attribute attribute, const QVariant &value)
{
    m_params.request.setAttribute(attribute, value);
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

HttpResponse::HttpResponse(HttpRequest::Params params, HttpRequest httpRequest)
    : QObject(params.reply),
      m_params(params),
      m_httpRequest(httpRequest),
      m_retriesRemaining(params.retryCount)
{
    int timeout = params.timeout;
    QNetworkReply *reply = params.reply;
    auto handleMap = params.handleMap;
    int isBlock = params.isBlock;

    new HttpResponseTimeout(this, timeout);

    connect(reply, SIGNAL(finished()),                         this, SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),   this, SLOT(onDownloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)),     this, SLOT(onUploadProgress(qint64, qint64)));
    connect(reply, SIGNAL(readyRead()),                        this, SLOT(onReadOnceReplyHeader()));
    connect(reply, SIGNAL(readyRead()),                        this, SLOT(onReadyRead()));

    connect(reply, SIGNAL(encrypted()),                 this, SLOT(onEncrypted()));
    connect(reply, SIGNAL(metaDataChanged()),           this, SLOT(onMetaDataChanged()));
    connect(reply, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)), this, SLOT(onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)));
    connect(reply, SIGNAL(redirectAllowed()),           this, SLOT(onRedirectAllowed()));
    connect(reply, SIGNAL(redirected(QUrl)),            this, SLOT(onRedirected(QUrl)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));

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

    // fixme
    for (auto each : handleMap.toStdMap()) {
        HandleType key                 = each.first;
        QPair<QString, QVariant> value = each.second;

        QVariant lambda      = value.second;
        QString lambdaString = value.first;

        const QObject *receiver = lambda.value<QObject *>();
        QString method          = lambdaString;

        if (key == h_onFinished) {

            if (lambdaString == T2S(std::function<void (QString)>)) {
                connect(this,
                        QOverload<QString>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QString)>>());
            }
            else if (lambdaString == T2S(std::function<void (QByteArray)>)) {
                connect(this,
                        QOverload<QByteArray>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QByteArray)>>());
            }
            else if (lambdaString == T2S(std::function<void (QVariantMap)>)) {
                connect(this,
                        QOverload<QVariantMap>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QVariantMap)>>());
            }
            else if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::finished),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(finished(QString)),
                    SIGNAL(finished(QByteArray)),
                    SIGNAL(finished(QVariantMap)),
                    SIGNAL(finished(QNetworkReply *)),
                };

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
        else if (key == h_onUploadProgress) {
            if (lambdaString == T2S(std::function<void (qint64, qint64)>)) {
                connect(this,
                        QOverload<qint64, qint64>::of(&HttpResponse::uploadProgress),
                        lambda.value<std::function<void (qint64, qint64)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(uploadProgress(qint64, qint64))
                };

                func(signalsList, receiver, method);
            }

        }
        else if (key == h_onError) {
            if (lambdaString == T2S(std::function<void (QString)>)) {
                connect(this,
                        QOverload<QString>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QString)>>());
            }
            else if (lambdaString == T2S(std::function<void (QByteArray)>)) {
                connect(this,
                        QOverload<QByteArray>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QByteArray)>>());
            }
            else if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else if (lambdaString == T2S(std::function<void (QNetworkReply::NetworkError)>)) {
                connect(this,
                        QOverload<QNetworkReply::NetworkError>::of(&HttpResponse::error),
                        lambda.value<std::function<void (QNetworkReply::NetworkError)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(error(QString)),
                    SIGNAL(error(QByteArray)),
                    SIGNAL(error(QNetworkReply *)),
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
                    SIGNAL(error()),
                    SIGNAL(error(QNetworkReply *))
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onReadyRead) {
            if (lambdaString == T2S(std::function<void (QNetworkReply*)>)) {
                connect(this,
                        QOverload<QNetworkReply *>::of(&HttpResponse::readyRead),
                        lambda.value<std::function<void (QNetworkReply*)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(readyRead(QNetworkReply *)),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onDownloadSuccess) {
            if (lambdaString == T2S(std::function<void (QString)>)) {
                connect(this,
                        QOverload<QString>::of(&HttpResponse::downloadFinished),
                        lambda.value<std::function<void (QString)>>());
            }
            else if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::downloadFinished),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(downloadFinished()),
                    SIGNAL(downloadFinished(QString)),
                };

                func(signalsList, receiver, method);
            }

        }
        else if (key == h_onDownloadFailed) {
            if (lambdaString == T2S(std::function<void (QString)>)) {
                connect(this,
                        QOverload<QString>::of(&HttpResponse::downloadError),
                        lambda.value<std::function<void (QString)>>());
            }
            else if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::downloadError),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(downloadError()),
                    SIGNAL(downloadError(QString)),
                };

                func(signalsList, receiver, method);
            }

        }
        else if (key == h_onEncrypted) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::encrypted),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(encrypted()),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onMetaDataChanged) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::metaDataChanged),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(metaDataChanged()),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onPreSharedKeyAuthenticationRequired) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<QSslPreSharedKeyAuthenticator*>::of(&HttpResponse::preSharedKeyAuthenticationRequired),
                        lambda.value<std::function<void (QSslPreSharedKeyAuthenticator*)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onRedirectAllowed) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::redirectAllowed),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(redirectAllowed()),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onRedirected) {
            if (lambdaString == T2S(std::function<void (QUrl)>)) {
                connect(this,
                        QOverload<QUrl>::of(&HttpResponse::redirected),
                        lambda.value<std::function<void (QUrl)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(redirected(QUrl)),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onSslErrors) {
            if (lambdaString == T2S(std::function<void (QList<QSslError>)>)) {
                connect(this,
                        QOverload<QList<QSslError>>::of(&HttpResponse::sslErrors),
                        lambda.value<std::function<void (QList<QSslError>)>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(sslErrors(QList<QSslError>)),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onRetried) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::retried),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(retried()),
                };

                func(signalsList, receiver, method);
            }
        }
        else if (key == h_onRepeated) {
            if (lambdaString == T2S(std::function<void ()>)) {
                connect(this,
                        QOverload<void>::of(&HttpResponse::repeated),
                        lambda.value<std::function<void ()>>());
            }
            else {
                QStringList signalsList = {
                    SIGNAL(repeated()),
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
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
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

    if (m_params.enabledRetry) {
        emit retried();
    }

    if (m_downloadFile.isOpen()) {
        emit downloadFinished();
        emit downloadFinished(m_downloadFile.fileName());

        m_downloadFile.close();
    }

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

    if (--m_params.repeatCount > 0) {
        HttpRequest httpRequest = m_httpRequest;
        httpRequest.repeat(m_params.repeatCount)
                   .exec();
    }
    else {
        emit repeated();
    }
}

void HttpResponse::onError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());

    if ( m_retriesRemaining-- > 0) {
        HttpRequest httpRequest = m_httpRequest;
        httpRequest.retry(m_retriesRemaining)
                   .enabledRetry(true)
                   .exec();
        reply->deleteLater();
        return;
    }

    if (m_params.enabledRetry) {
        emit retried();
    }

    const QMetaObject & metaObject = QNetworkReply::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("NetworkError"));
    QString errorString = reply->errorString().isEmpty() ? metaEnum.valueToKey(error) : reply->errorString();

    if (m_params.downloadFile.first == HttpRequest::Params::DownloadEnabled::Enabled) {
        QString error = QString("Url: %1 file: %2 error: %3")
                .arg(m_params.request.url().toString()) // fixme
                .arg(m_downloadFile.fileName())
                .arg(errorString);

        emit downloadError();
        emit downloadError(error);

        m_downloadFile.close();
    }

    if (this->receivers(SIGNAL(error(QNetworkReply*))) > 0) {
        emit this->error(reply);
        return;
    }

    emit this->error(errorString);
    emit this->error(error);
    emit this->error(errorString.toLocal8Bit());

    reply->deleteLater();

    if (--m_params.repeatCount > 0) {
        HttpRequest httpRequest = m_httpRequest;
        httpRequest.repeat(m_params.repeatCount)
                   .exec();
    }
    else {
        emit repeated();
    }
}

void HttpResponse::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit this->downloadProgress(bytesReceived, bytesTotal);
}

void HttpResponse::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    emit this->uploadProgress(bytesSent, bytesTotal);
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

void HttpResponse::onReadyRead()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());

    if (m_params.downloadFile.first == HttpRequest::Params::Enabled) {
        if (m_downloadFile.isOpen()){
            int size = m_downloadFile.write(reply->readAll());
            if (size == -1) {
                QString error = QString("Url: %1 %2 Write failed!")
                                .arg(m_params.request.url().toString())
                                .arg(m_downloadFile.fileName());
                emit downloadError();
                emit downloadError(error);
            }
        }
        else {
            // do nothing
        }
    }
    else {
        // do nothing
    }

    emit readyRead(reply);
}

void HttpResponse::onReadOnceReplyHeader()
{
    if (m_params.downloadFile.first != HttpRequest::Params::DownloadEnabled::Enabled)
        return;

    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(onReadOnceReplyHeader()));

    QString fileName = m_params.downloadFile.second;
    if (fileName.isEmpty()) {
        // fixme utf8
        QString dispositionHeader = QString::fromUtf8(reply->header(QNetworkRequest::ContentDispositionHeader).toByteArray());

        // fixme rx
        QRegExp rx("attachment;\\sfilename=([\\S]+)");
        if (rx.exactMatch(dispositionHeader)) {
            fileName = rx.cap(1);
        }
        else {
            fileName = m_params.request.url().fileName();
        }

        _debugger << "start downloading: url:" << reply->url().toString() << "fileName: " << fileName;
    }

    m_downloadFile.setFileName(fileName);

    if (!m_downloadFile.open(QIODevice::WriteOnly)) {
        QString error = QString("Url: %1 %2 Non-Writable")
                .arg(m_params.request.url().toString())
                .arg(m_downloadFile.fileName());
        emit downloadError();
        emit downloadError(error);
    }
    else {
        // todo startDownload
    }
}

void HttpResponse::onEncrypted()
{
    emit encrypted();
}

void HttpResponse::onMetaDataChanged()
{
    emit metaDataChanged();
}

void HttpResponse::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    emit preSharedKeyAuthenticationRequired(authenticator);
}

void HttpResponse::onRedirectAllowed()
{
    emit redirectAllowed();
}

void HttpResponse::onRedirected(const QUrl &url)
{
    emit redirected(url);
}

void HttpResponse::onSslErrors(const QList<QSslError> &errors)
{
    emit sslErrors(errors);
}

}

Q_DECLARE_METATYPE(std::function<void ()>)
Q_DECLARE_METATYPE(std::function<void (QByteArray)>)
Q_DECLARE_METATYPE(std::function<void (QString)>)
Q_DECLARE_METATYPE(std::function<void (QVariantMap)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply*)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError)>)
Q_DECLARE_METATYPE(std::function<void (qint64, qint64)>)
Q_DECLARE_METATYPE(std::function<void (QSslPreSharedKeyAuthenticator *)>)
Q_DECLARE_METATYPE(std::function<void (QUrl)>)
Q_DECLARE_METATYPE(std::function<void (QList<QSslError>)>)

#endif // HTTPCLIENT_HPP
