﻿/**********************************************************
 * Author(作者)     : Qt君
 * 微信公众号        : Qt君
 * Website(网站)    : qthub.com
 * QQ交流群         : 1039852727
 * Email(邮箱)      : 2088201923@qq.com
 * Support(技术支持&合作) :2088201923(QQ)
 * Source Code(源码): https://github.com/aeagean/QtNetworkService
 * LISCENSE(开源协议): MIT
 add createInstanceForAutoDelete
 ==========================================================
   static AeaQt::HttpClient client;
   client.get("https://qthub.com")
         .onSuccess([](QString result) { qDebug()<<"success!"; })
         .onFailed([](QString error) { qDebug()<<"failed!"; })
         .exec();
   or 

   AeaQt::HttpClient* client = AeaQt::HttpClient::createInstanceForAutoDelete();
   client->get("https://qthub.com")
         .onSuccess([](QString result) { qDebug()<<"success!"; })
		 .onFailed([](QString error) { qDebug()<<"failed!"; })
         .exec();
 ==========================================================
**********************************************************/
#ifndef QTHUB_COM_HTTPCLIENT_HPP
#define QTHUB_COM_HTTPCLIENT_HPP

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QAuthenticator>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

#include <QtCore/QBuffer>
#include <QtCore/QMetaEnum>
#include <QtCore/QUrlQuery>
#include <QtCore/QFileInfo>

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>
#include <QtCore/QDebug>

#include <functional>

namespace AeaQt {

class HttpClient;
class HttpRequest;
class HttpResponse;

class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    inline static HttpClient *instance();
	inline static HttpClient *createInstanceForAutoDelete();
    inline HttpClient();

    inline HttpRequest head(const QString &url);
    inline HttpRequest get(const QString &url);
    inline HttpRequest post(const QString &url);
    inline HttpRequest put(const QString &url);

    inline HttpRequest send(const QString &url, Operation op = GetOperation);

private:
#if (QT_VERSION < QT_VERSION_CHECK(5, 8, 0))
    inline QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data);
    inline QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QHttpMultiPart *multiPart);
#endif
	bool m_bAutoDelete = false;
    friend class HttpRequest;
};

class HttpRequest
{
public:
    inline explicit HttpRequest(QNetworkAccessManager::Operation op, HttpClient *httpClient);
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
    inline HttpRequest &bodyWithFormUrlencoded(const QString &key, const QVariant &value);
    inline HttpRequest &bodyWithFormUrlencoded(const QVariantMap &keyValueMap);

    inline HttpRequest &bodyWithFormData(const QString &key, const QVariant &value);
    inline HttpRequest &bodyWithFormData(const QVariantMap &keyValueMap);

    inline HttpRequest &body(QHttpMultiPart *multiPart);
    inline HttpRequest &bodyWithMultiPart(QHttpMultiPart *multiPart);

    // multi-params
    inline HttpRequest &body(const QString &key, const QString &file);
    inline HttpRequest &bodyWithFile(const QString &key, const QString &file);
    inline HttpRequest &bodyWithFile(const QMap<QString/*key*/, QString/*file*/> &fileMap); // => QMap<key, file>; like: { "key": "/home/example/car.jpeg" }

    inline HttpRequest &ignoreSslErrors(const QList<QSslError> &errors);
    inline HttpRequest &sslConfiguration(const QSslConfiguration &config);

    inline HttpRequest &priority(QNetworkRequest::Priority priority);
    inline HttpRequest &maximumRedirectsAllowed(int maxRedirectsAllowed);
    inline HttpRequest &originatingObject(QObject *object);
    inline HttpRequest &readBufferSize(qint64 size);

    // Authentication
    inline HttpRequest &autoAuthenticationRequired(const QAuthenticator &authenticator);
    inline HttpRequest &autoAuthenticationRequired(const QString &user, const QString &password);

    // 超过身份验证计数则触发失败并中断请求。
    // count >= 0 => count
    // count < 0 => infinite
    inline HttpRequest &authenticationRequiredCount(int count = 1);

    /**
     * @brief msec <= 0, disable timeout
     *        msec >  0, enable timeout
     */
    inline HttpRequest &timeout(const int &second = -1);
    inline HttpRequest &timeoutMs(const int &msec = -1);

    inline HttpRequest &download();
    inline HttpRequest &download(const QString &file);
    inline HttpRequest &enabledBreakpointDownload(bool enabled = true);

    inline HttpRequest &retry(int count);
    inline HttpRequest &repeat(int count);

    /**
     * @brief Block(or sync) current thread, entering an event loop.
     */
    inline HttpRequest &block(bool isBlock = true);
    inline HttpRequest &sync(bool isSync = true);

    inline HttpResponse *exec();

    // onFinished == onSuccess
    inline HttpRequest &onSuccess(const QObject *receiver, const char *method);
    inline HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);
    inline HttpRequest &onSuccess(std::function<void ()> lambda);

    // onFinished == onSuccess
    inline HttpRequest &onFinished(const QObject *receiver, const char *method);
    inline HttpRequest &onFinished(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onFinished(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onFinished(std::function<void (QByteArray)> lambda);
    inline HttpRequest &onFinished(std::function<void ()> lambda);

    // onError == onFailed
    inline HttpRequest &onError(const QObject *receiver, const char *method);
    inline HttpRequest &onError(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onError(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onError(std::function<void (QByteArray)> lambda);
    inline HttpRequest &onError(std::function<void ()> lambda);

    // onError == onFailed
    inline HttpRequest &onFailed(const QObject *receiver, const char *method);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onFailed(std::function<void (QNetworkReply::NetworkError)> lambda);
    inline HttpRequest &onFailed(std::function<void (QByteArray)> lambda);
    inline HttpRequest &onFailed(std::function<void ()> lambda);

    inline HttpRequest &onReadyRead(const QObject *receiver, const char *method);
    inline HttpRequest &onReadyRead(std::function<void (QNetworkReply*)> lambda);

    inline HttpRequest &onHead(const QObject *receiver, const char *method);
    inline HttpRequest &onHead(std::function<void (QList<QNetworkReply::RawHeaderPair>)> lambda);
    inline HttpRequest &onHead(std::function<void (QMap<QString, QString>)> lambda);

    inline HttpRequest &onTimeout(const QObject *receiver, const char *method);
    inline HttpRequest &onTimeout(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onTimeout(std::function<void ()> lambda);

    inline HttpRequest &onUploadProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onUploadProgress(std::function<void (qint64, qint64)> lambda);

    inline HttpRequest &onDownloadProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadProgress(std::function<void (qint64, qint64)> lambda);

    /// file download interface
    inline HttpRequest &onDownloadFileNameChanged(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadFileNameChanged(std::function<void (QString/*fileName*/)> lambda);

    inline HttpRequest &onDownloadFileProgress(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadFileProgress(std::function<void (qint64, qint64)> lambda);

    inline HttpRequest &onDownloadFileSuccess(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadFileSuccess(std::function<void ()> lambda);
    inline HttpRequest &onDownloadFileSuccess(std::function<void (QString/*fileName*/)> lambda);

    inline HttpRequest &onDownloadFileFailed(const QObject *receiver, const char *method);
    inline HttpRequest &onDownloadFileFailed(std::function<void ()> lambda);
    inline HttpRequest &onDownloadFileFailed(std::function<void (QString/*fileName*/)> lambda);
    /// file download interface

    inline HttpRequest &onRetried(const QObject *receiver, const char *method);
    inline HttpRequest &onRetried(std::function<void ()> lambda);

    inline HttpRequest &onRepeated(const QObject *receiver, const char *method);
    inline HttpRequest &onRepeated(std::function<void ()> lambda);

    inline HttpRequest &onAuthenticationRequired(const QObject *receiver, const char *method);
    inline HttpRequest &onAuthenticationRequired(std::function<void (QAuthenticator *)> lambda);

    inline HttpRequest &onAuthenticationRequireFailed(const QObject *receiver, const char *method);
    inline HttpRequest &onAuthenticationRequireFailed(std::function<void ()> lambda);
    inline HttpRequest &onAuthenticationRequireFailed(std::function<void (QNetworkReply *)> lambda);

    // onResponse == onSuccess. note: DEPRECATED
    inline HttpRequest &onResponse(const QObject *receiver, const char *method);
    inline HttpRequest &onResponse(std::function<void (QNetworkReply*)> lambda);
    inline HttpRequest &onResponse(std::function<void (QVariantMap)> lambda);
    inline HttpRequest &onResponse(std::function<void (QByteArray)> lambda);

    enum HandleType {
        h_onFinished = 0,
        h_onError,
        h_onDownloadProgress,
        h_onUploadProgress,
        h_onTimeout,
        h_onReadyRead,
        h_onDownloadFileSuccess,
        h_onDownloadFileFailed,
        h_onEncrypted,
        h_onMetaDataChanged,
        h_onPreSharedKeyAuthenticationRequired,
        h_onRedirectAllowed,
        h_onRedirected,
        h_onSslErrors,
        h_onRetried,
        h_onRepeated,
        h_onAuthenticationRequired,
        h_onAuthenticationRequireFailed,
        h_onHead,
        h_onDownloadFileProgess,
        h_onDownloadFileNameChanged,
    };

protected:
    struct Params {
        enum BodyType {
            None = 0,              // This request does not have a body.
            Raw,
            Raw_Json,              // application/json
            X_Www_Form_Urlencoded, // x-www-form-urlencoded
            FileMap,               // multipart/form-data
            MultiPart,             // multipart/form-data
            FormData               // multipart/form-data
        };

        struct Downloader {
            bool    isEnabled;
            QString fileName;
            bool    enabledBreakpointDownload;
            bool    isSupportBreakpointDownload;
            qint64  currentSize;
            qint64  totalSize;

            Downloader() {
                isEnabled = false;
                fileName = "";
                enabledBreakpointDownload = true;
                isSupportBreakpointDownload = false;
                totalSize = 0;
                currentSize = 0;
            }
        };

        QNetworkAccessManager::Operation op;
        HttpClient                      *httpClient;
        QNetworkReply                   *reply;
        QNetworkRequest                  request;

        QPair<BodyType, QVariant>        body;
        int                              timeoutMs; // ms
        bool                             isBlock;
        int                              retryCount;
        bool                             enabledRetry;
        int                              repeatCount;
        QAuthenticator                   authenticator;
        int                              authenticationRequiredCount;

        qint64                                              readBufferSize;
        QList<QSslError>                                    ignoreSslErrors;
        Downloader                                          downloader;
        QMap<HandleType, QList<QPair<QString, QVariant> > > handleMap;

        Params()
        {
            reply          = nullptr;
            httpClient     = nullptr;
            isBlock        = false;
            retryCount     = 0;
            repeatCount    = 1;
            enabledRetry   = false;
            timeoutMs      = -1;
            body           = qMakePair(BodyType::None, QByteArray());
            readBufferSize = 0;
            authenticationRequiredCount = 1;
        }
    };

protected:
    inline HttpRequest &enabledRetry(bool isEnabled);
	inline bool isAutoDeleteClient() { return m_params.httpClient->m_bAutoDelete; }

    friend class HttpResponse;

private:
    inline HttpRequest() = delete;
    inline HttpRequest &onResponse(HandleType type, const QObject *receiver, const char *method);
    inline HttpRequest &onResponse(HandleType type, QVariant lambda);
    inline HttpRequest &onResponse(HandleType type, QString key, QVariant value);

    inline HttpResponse *_exec(Params params);

private:
    Params m_params;
    QVariantMap m_formUrlencodedMap;
    QVariantMap m_formDataMap;
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
    void finished();
    void finished(QByteArray result);
    void finished(QVariantMap resultMap);

    void downloadProgress(qint64, qint64);
    void uploadProgress(qint64, qint64);

    void error(QByteArray error);
    void error();
    void error(QNetworkReply::NetworkError error);
    void error(QNetworkReply *reply);

    void timeout();
    void timeout(QNetworkReply *reply);

    void readyRead(QNetworkReply *reply);

    void downloadFileNameChanged(QString fileName);

    void downloadFileFinished();
    void downloadFileFinished(QString file);

    void downloadFileError();
    void downloadFileError(QString errorString);

    void encrypted();
    void metaDataChanged();

    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void redirectAllowed();
    void redirected(QUrl url);
    void sslErrors(QList<QSslError> errors);

    void retried();
    void repeated();

    void authenticationRequired(QAuthenticator *authentication);
    void authenticationRequireFailed();
    void authenticationRequireFailed(QNetworkReply *);

    void head(QList<QNetworkReply::RawHeaderPair>);
    void head(QMap<QString, QString>);

    void downloadFileProgress(qint64 bytesReceived, qint64 bytesTotal);

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

    inline void onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

    inline void onHandleHead();

private:
    HttpRequest::Params m_params;
    HttpRequest         m_httpRequest;
    QFile               m_downloadFile;
    int                 m_retriesRemaining = 0;
    int                 m_authenticationCount = 0;
    bool                m_isHandleHead = false;
};

class HttpResponseTimeout : public QObject {
    Q_OBJECT
public:
    HttpResponseTimeout(HttpResponse *parent, const int timeout = -1) : QObject(parent) {
        if (timeout > 0)
            QTimer::singleShot(timeout, parent, SLOT(onTimeout()));
    }
};

class HttpBlocker: public QEventLoop {
    Q_OBJECT
public:
    HttpBlocker(QNetworkReply *reply, bool isBlock) : QEventLoop(reply) {
        if (isBlock) {
            connect(reply, SIGNAL(finished()), this, SLOT(quit()));
            this->exec();
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

HttpRequest &HttpRequest::bodyWithFormUrlencoded(const QString &key, const QVariant &value)
{
    QVariantMap map;
    map[key] = value;

    return bodyWithFormUrlencoded(map);
}

HttpRequest &HttpRequest::bodyWithFormUrlencoded(const QVariantMap &keyValueMap)
{
    // merge map
    for (auto each : keyValueMap.toStdMap()) {
        m_formUrlencodedMap[each.first] = each.second;
    }

    QMapIterator<QString, QVariant> i(m_formUrlencodedMap);

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

HttpRequest &HttpRequest::bodyWithFormData(const QString &key, const QVariant &value)
{
    QVariantMap map;
    map[key] = value;

    return bodyWithFormData(map);
}

HttpRequest &HttpRequest::bodyWithFormData(const QVariantMap &keyValueMap)
{
    // merge map
    for (auto each : keyValueMap.toStdMap()) {
        m_formDataMap[each.first] = each.second;
    }

    m_params.body = qMakePair(Params::FormData, m_formDataMap);
    m_params.request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    this->attribute(QNetworkRequest::RedirectPolicyAttribute, true);
#else
    this->attribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif

    m_params.downloader.isEnabled = true;
    m_params.downloader.fileName = file;
    return *this;
}

HttpRequest &HttpRequest::enabledBreakpointDownload(bool enabled)
{
    m_params.downloader.enabledBreakpointDownload = enabled;
    return *this;
}

HttpRequest &HttpRequest::onDownloadFileProgress(const QObject *receiver, const char *method)
{
    return onResponse(h_onDownloadFileProgess, receiver, method);
}

HttpRequest &HttpRequest::onDownloadFileProgress(std::function<void (qint64, qint64)> lambda)
{
    return onResponse(h_onDownloadFileProgess, QVariant::fromValue(lambda));
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

HttpRequest &HttpRequest::priority(QNetworkRequest::Priority priority)
{
    m_params.request.setPriority(priority);
    return *this;
}

HttpRequest &HttpRequest::maximumRedirectsAllowed(int maxRedirectsAllowed)
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

HttpRequest &HttpRequest::autoAuthenticationRequired(const QAuthenticator &authenticator)
{
    m_params.authenticator = authenticator;
    return *this;
}

HttpRequest &HttpRequest::autoAuthenticationRequired(const QString &user, const QString &password)
{
    QAuthenticator a;
    a.setUser(user);
    a.setPassword(password);

    return autoAuthenticationRequired(a);
}

HttpRequest &HttpRequest::authenticationRequiredCount(int count)
{
    m_params.authenticationRequiredCount = count;
    return *this;
}

// event [0]
HttpRequest &HttpRequest::timeout(const int &second) { return timeoutMs(second * 1000); }
HttpRequest &HttpRequest::timeoutMs(const int &msec) { m_params.timeoutMs = msec; return *this; }

HttpRequest &HttpRequest::retry(int count) { m_params.retryCount = count; return *this; }

HttpRequest &HttpRequest::repeat(int count) { m_params.repeatCount = count; return *this; }

HttpRequest &HttpRequest::block(bool isBlock) { m_params.isBlock = isBlock; return *this; }
HttpRequest &HttpRequest::sync(bool isSync) { return block(isSync); }

HttpRequest &HttpRequest::onFinished(const QObject *receiver, const char  *method) { return onResponse(h_onFinished, receiver, method); }
HttpRequest &HttpRequest::onFinished(std::function<void (QNetworkReply *)> lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onFinished(std::function<void (QVariantMap)>     lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onFinished(std::function<void (QByteArray)>      lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onFinished(std::function<void ()>                lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onSuccess(const QObject *receiver, const char  *method) { return onFinished(receiver, method); }
HttpRequest &HttpRequest::onSuccess(std::function<void (QNetworkReply *)> lambda) { return onFinished(lambda); }
HttpRequest &HttpRequest::onSuccess(std::function<void (QVariantMap)>     lambda) { return onFinished(lambda); }
HttpRequest &HttpRequest::onSuccess(std::function<void (QByteArray)>      lambda) { return onFinished(lambda); }
HttpRequest &HttpRequest::onSuccess(std::function<void ()>                lambda) { return onFinished(lambda); }

HttpRequest &HttpRequest::onFailed(const QObject *receiver,              const char *method) { return onError(receiver, method); }
HttpRequest &HttpRequest::onFailed(std::function<void (QNetworkReply *)>             lambda) { return onError(lambda); }
HttpRequest &HttpRequest::onFailed(std::function<void (QNetworkReply::NetworkError)> lambda) { return onError(lambda); }
HttpRequest &HttpRequest::onFailed(std::function<void (QByteArray)>                  lambda) { return onError(lambda); }
HttpRequest &HttpRequest::onFailed(std::function<void ()>                            lambda) { return onError(lambda); }

HttpRequest &HttpRequest::onError(const QObject *receiver,              const char *method) { return onResponse(h_onError, receiver, method); }
HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply *)>             lambda) { return onResponse(h_onError, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onError(std::function<void (QNetworkReply::NetworkError)> lambda) { return onResponse(h_onError, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onError(std::function<void (QByteArray)>                  lambda) { return onResponse(h_onError, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onError(std::function<void ()>                            lambda) { return onResponse(h_onError, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onReadyRead(const QObject *receiver, const char  *method) { return onResponse(h_onReadyRead, receiver, method); }
HttpRequest &HttpRequest::onReadyRead(std::function<void (QNetworkReply *)> lambda) { return onResponse(h_onReadyRead, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onHead(const QObject *receiver, const char *method) { return onResponse(h_onHead, receiver, method); }
HttpRequest &HttpRequest::onHead(std::function<void (QList<QNetworkReply::RawHeaderPair>)> lambda) { return onResponse(h_onHead, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onHead(std::function<void (QMap<QString, QString>)> lambda) { return onResponse(h_onHead, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onDownloadProgress(const QObject *receiver, const char *method) { return onResponse(h_onDownloadProgress, receiver, method); }
HttpRequest &HttpRequest::onDownloadProgress(std::function<void (qint64, qint64)> lambda) { return onResponse(h_onDownloadProgress, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onDownloadFileNameChanged(const QObject *receiver, const char *method) { return onResponse(h_onDownloadFileNameChanged, receiver, method); }
HttpRequest &HttpRequest::onDownloadFileNameChanged(std::function<void (QString)> lambda) { return onResponse(h_onDownloadFileNameChanged, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onDownloadFileSuccess(const QObject *receiver, const char *method) { return onResponse(h_onDownloadFileSuccess, receiver, method); }
HttpRequest &HttpRequest::onDownloadFileSuccess(std::function<void ()>               lambda) { return onResponse(h_onDownloadFileSuccess, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onDownloadFileSuccess(std::function<void (QString)>        lambda) { return onResponse(h_onDownloadFileSuccess, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onDownloadFileFailed(const QObject *receiver, const char *method) { return onResponse(h_onDownloadFileFailed, receiver, method); }
HttpRequest &HttpRequest::onDownloadFileFailed(std::function<void ()>               lambda) { return onResponse(h_onDownloadFileFailed, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onDownloadFileFailed(std::function<void (QString)>        lambda) { return onResponse(h_onDownloadFileFailed, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onUploadProgress(const QObject *receiver, const char *method) { return onResponse(h_onUploadProgress, receiver, method); }
HttpRequest &HttpRequest::onUploadProgress(std::function<void (qint64, qint64)> lambda) { return onResponse(h_onUploadProgress, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onTimeout(const QObject *receiver, const char  *method) { return onResponse(h_onTimeout, receiver, method); }
HttpRequest &HttpRequest::onTimeout(std::function<void (QNetworkReply *)> lambda) { return onResponse(h_onTimeout, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onTimeout(std::function<void ()>                lambda) { return onResponse(h_onTimeout, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onRetried(const QObject *receiver, const char  *method) { return onResponse(h_onRetried, receiver, method); }
HttpRequest &HttpRequest::onRetried(std::function<void ()>                lambda) { return onResponse(h_onRetried, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onRepeated(const QObject *receiver, const char *method) { return onResponse(h_onRepeated, receiver, method); }
HttpRequest &HttpRequest::onRepeated(std::function<void ()>               lambda) { return onResponse(h_onRepeated, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onAuthenticationRequired(const QObject *receiver, const char *method)   { return onResponse(h_onAuthenticationRequired, receiver, method); }
HttpRequest &HttpRequest::onAuthenticationRequired(std::function<void (QAuthenticator *)> lambda) { return onResponse(h_onAuthenticationRequired, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onAuthenticationRequireFailed(const QObject *receiver, const char *method) { return onResponse(h_onAuthenticationRequireFailed, receiver, method); }
HttpRequest &HttpRequest::onAuthenticationRequireFailed(std::function<void ()> lambda) { return onResponse(h_onAuthenticationRequireFailed, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onAuthenticationRequireFailed(std::function<void (QNetworkReply *)> lambda) { return onResponse(h_onAuthenticationRequireFailed, QVariant::fromValue(lambda)); }

HttpRequest &HttpRequest::onResponse(const QObject *receiver, const char *method) { return onResponse(h_onFinished, receiver, method); }
HttpRequest &HttpRequest::onResponse(std::function<void (QNetworkReply*)> lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onResponse(std::function<void (QVariantMap)>    lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onResponse(std::function<void (QByteArray)>     lambda) { return onResponse(h_onFinished, QVariant::fromValue(lambda)); }
HttpRequest &HttpRequest::onResponse(HandleType type, QVariant            lambda) { return onResponse(type, lambda.typeName(), lambda); }
HttpRequest &HttpRequest::onResponse(HandleType type, const QObject *receiver, const char *method) { return onResponse(type, method, QVariant::fromValue((QObject *)receiver)); }
HttpRequest &HttpRequest::onResponse(HandleType type, QString key, QVariant value)
{
    if (!m_params.handleMap.contains(type)) {
        QList<QPair<QString, QVariant> > handleList;
        m_params.handleMap.insert(type, handleList);
    }

    auto handleList = m_params.handleMap[type];
    handleList.append({key, value});

    m_params.handleMap.insert(type, handleList);
    return *this;
}

HttpResponse *HttpRequest::_exec(Params params)
{
#ifdef QT_APP_DEBUG
    _debugger << "Http Client info: ";
    _debugger << "Url: " << params.request.url().toString();
    _debugger << "Type: " << params.op;
    QString headers;
    for (int i = 0; i < params.request.rawHeaderList().count(); i++) {
        QString each = params.request.rawHeaderList().at(i);
        QString header = params.request.rawHeader(each.toUtf8());
        headers += QString("%1: %2;").arg(each)
                                     .arg(header);
    }

    _debugger << "Header: " << headers;
    _debugger << "Body:\r\n" << params.body;
#endif

    static QMap<QNetworkAccessManager::Operation, QByteArray> verbMap = {
        {QNetworkAccessManager::HeadOperation, "HEAD"},
        {QNetworkAccessManager::GetOperation,  "GET"},
        {QNetworkAccessManager::PostOperation, "POST"},
        {QNetworkAccessManager::PutOperation,  "PUT"},
    };

    if (!verbMap.contains(params.op)) {
        qWarning() << "Url: [" << params.request.url().toString() << "]" << params.op << "not support!";
        return nullptr;
    }

    using BodyType = HttpRequest::Params::BodyType;
    BodyType        bodyType = params.body.first;
    QVariant           &body = params.body.second;
    QNetworkRequest &request = params.request;
    HttpClient   *httpClient = params.httpClient;

    if (bodyType == BodyType::MultiPart) {
        QHttpMultiPart *multiPart = body.value<QHttpMultiPart*>();
        QString       contentType = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());

        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

        params.reply = params.httpClient->sendCustomRequest(request,
                                                       verbMap.value(params.op),
                                                       multiPart);
        multiPart->setParent(params.reply);

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

        params.reply = httpClient->sendCustomRequest(request,
                                              verbMap.value(params.op),
                                              multiPart);
        if (params.reply)
            multiPart->setParent(params.reply);
        else
            delete multiPart;

    }
    else if (bodyType == BodyType::FormData) {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QString       contentType = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());
        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

        const auto &formDataMap = body.value<QMap<QString, QVariant>>();
        for (const auto &each : formDataMap.toStdMap()) {
            const QString &key      = each.first;
            const QString &value    = each.second.toString();

            QString dispositionHeader = QString("form-data; name=\"%1\"").arg(key);

            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader, dispositionHeader);
            part.setBody(value.toUtf8());

            multiPart->append(part);
        }

        params.reply = httpClient->sendCustomRequest(request,
                                              verbMap.value(params.op),
                                              multiPart);

        if (params.reply)
            multiPart->setParent(params.reply);
        else
            delete multiPart;
    }
    else {
        params.reply = params.httpClient->sendCustomRequest(request,
                                                            verbMap.value(params.op),
                                                            body.toByteArray());
    }

    if (params.reply == nullptr) {
        // fixme: todo onError
        qWarning() << "Http reply invalid";
        Q_ASSERT(params.reply);
        return nullptr;
    }

    // fixme
    params.reply->ignoreSslErrors(params.ignoreSslErrors);
    params.reply->setReadBufferSize(params.readBufferSize);

    return new HttpResponse(params, *this);
}

// event [0]

inline QDebug &operator<<(QDebug &debug, const QNetworkAccessManager::Operation &op)
{
    switch (op) {
        case QNetworkAccessManager::HeadOperation: return debug  << "HeadOperation";
        case QNetworkAccessManager::GetOperation:  return debug  << "GetOperation";
        case QNetworkAccessManager::PostOperation: return debug  << "PostOperation";
        case QNetworkAccessManager::PutOperation:  return debug  << "PutOperation";
        case QNetworkAccessManager::DeleteOperation: return debug  << "DeleteOperation";
        case QNetworkAccessManager::CustomOperation: return debug  << "CustomOperation";
        default: return debug  << "UnknownOperation";
    }
}

template<typename T>
inline T &operator<<(T &debug, const HttpRequest::HandleType &handleType)
{
    switch (handleType) {
        case HttpRequest::h_onFinished: return debug << "onFinished";
        case HttpRequest::h_onError:    return debug << "onError";
        case HttpRequest::h_onDownloadProgress: return debug << "onDownloadProgress";
        case HttpRequest::h_onUploadProgress:   return debug << "onUploadProgress";
        // todo: onUploadProgressSuccess and onUploadProgressFaied
        case HttpRequest::h_onDownloadFileSuccess:  return debug << "onDownloadFileSuccess";
        case HttpRequest::h_onDownloadFileFailed:   return debug << "onDownloadFileFailed";
        case HttpRequest::h_onTimeout:          return debug << "onTimeout";
        case HttpRequest::h_onReadyRead:        return debug << "onReadyRead";
        case HttpRequest::h_onEncrypted:        return debug << "onEncrypted";
        case HttpRequest::h_onMetaDataChanged:  return debug << "onMetaChanged";
        case HttpRequest::h_onPreSharedKeyAuthenticationRequired: return debug << "onPreSharedKeyAuthenticationRequired";
        case HttpRequest::h_onRedirectAllowed:  return debug << "onRedirectAllowed";
        case HttpRequest::h_onRedirected:       return debug << "onRedirected";
        case HttpRequest::h_onSslErrors:        return debug << "onSslErrors";
        case HttpRequest::h_onRetried:          return debug << "onRetried";
        case HttpRequest::h_onRepeated:         return debug << "onRepeated";
        case HttpRequest::h_onAuthenticationRequired:      return debug << "onAuthenticationRequired";
        case HttpRequest::h_onAuthenticationRequireFailed: return debug << "onAuthenticationRequireFailed";
        case HttpRequest::h_onHead:                    return debug << "onHead";
        case HttpRequest::h_onDownloadFileProgess:     return debug << "onDownloadFileProgress";
        case HttpRequest::h_onDownloadFileNameChanged: return debug << "onDownloadFileNameChanged";
        default: return debug << "Unknow";
    }
}

static int extractCode(const char *member)
{
    /* extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE */
    return (((int)(*member) - '0') & 0x3);
}

static bool isMethod(const char *member)
{
    int ret = extractCode(member);
    return  ret >= QMETHOD_CODE && ret <= QSIGNAL_CODE;
}


template<typename M, typename L, typename T>
bool httpResponseConnect(L sender, T senderSignal, const QString &lambdaString, const QVariant &lambda)
{
    if (lambdaString == QVariant::fromValue(M()).typeName()) {
        return QObject::connect(sender, senderSignal, lambda.value<M>());
    }
    else if (isMethod(qPrintable(lambdaString))) {
        QString signal = QMetaMethod::fromSignal(senderSignal).methodSignature();
        signal.insert(0, "2");
        signal.replace("qlonglong", "qint64");

        const QObject *receiver = lambda.value<QObject*>();
        QString          method = QMetaObject::normalizedSignature(qPrintable(lambdaString)); // remove 'const', like: const QString => QString

        if (QMetaObject::checkConnectArgs(qPrintable(signal), qPrintable(method))) {
            return QObject::connect(sender, qPrintable(signal), receiver, qPrintable(method));
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

#define HTTP_RESPONSE_CONNECT_X(sender, senderSignal, lambdaString, lambda, ...) \
    httpResponseConnect< std::function<void (__VA_ARGS__)> > ( \
             sender, \
             static_cast<void (HttpResponse::*)(__VA_ARGS__)>(&HttpResponse::senderSignal), \
             lambdaString, \
             lambda);

HttpResponse *HttpRequest::exec()
{
    Params params = m_params;
    if (params.downloader.isEnabled) {
        bool isSupportContinueDownload = false;
        QString fileName = params.request.url().fileName();
        qint64 contentLength = 0;
        QList<QNetworkReply::RawHeaderPair> headerForPair;
        QMap<QString, QString> headerForMap;
        HttpResponse *response = new HttpResponse(params, *this);

        std::function<void (QList<QNetworkReply::RawHeaderPair>)> funcOnHead = [&](QList<QNetworkReply::RawHeaderPair> _headerForPair) {
            headerForPair = _headerForPair;
       };
        std::function<void (QMap<QString, QString>)> funcOnHead2 = [&](QMap<QString, QString> _headerForMap){
            headerForMap = _headerForMap;
            for (auto each: headerForMap.toStdMap()) {
                QString key = each.first;
                QString value = each.second;

                if (key.contains("Content-Disposition", Qt::CaseInsensitive)) {
                    QString dispositionHeader = value;
                    // fixme rx
                    QRegExp rx("attachment;\\s*filename=([\\S]+)");
                    if (rx.exactMatch(dispositionHeader)) {
                        fileName = rx.cap(1);
                    }
                }

                if (key.contains("Content-Length", Qt::CaseInsensitive)) {
                    contentLength = value.toLongLong();
                }

                if (key.contains("Content-Range", Qt::CaseInsensitive) ||
                    key.contains("Accept-Ranges", Qt::CaseInsensitive)) {
                    isSupportContinueDownload = true;
                }
            }
       };

        HttpClient client;
        client.get(params.request.url().toString())
              .timeout(30)
              .attribute(QNetworkRequest::FollowRedirectsAttribute, true)
              .onHead(funcOnHead)
              .onHead(funcOnHead2)
              .onReadyRead([](QNetworkReply *reply){ reply->abort(); })
              //.onSuccess([](QByteArray s){qDebug()<<"s:" << s;})
              //.onFailed([](QString s){qDebug()<<"f:" << s;})
              .block()
              .exec();

        if (params.downloader.fileName.isEmpty()) {
            params.downloader.fileName = fileName;
            emit response->downloadFileNameChanged(fileName);
        }

        params.downloader.totalSize = contentLength;
        params.downloader.isSupportBreakpointDownload = isSupportContinueDownload;

        if (params.downloader.enabledBreakpointDownload && isSupportContinueDownload) {
            QFile file(params.downloader.fileName);
            if (file.exists() && file.open(QIODevice::ReadOnly)) {
                params.downloader.currentSize = file.size();

                if (file.size() == contentLength) {
                    emit response->head(headerForPair);
                    emit response->head(headerForMap);

                    emit response->downloadFileProgress(contentLength, contentLength);

                    emit response->downloadFileFinished();
                    emit response->downloadFileFinished(params.downloader.fileName);

                    emit response->finished();
                    emit response->finished(QByteArray(""));
                    emit response->finished(QVariantMap{});
                    emit response->finished(nullptr);

                    response->deleteLater();
                    return response;
                }
                else if (file.size() > contentLength) {
                    file.close();
                    file.open(QIODevice::Truncate);
                    file.close();
                }
                else {
                    params.request.setRawHeader("Range", QString("bytes=%1-").arg(file.size()).toUtf8());
                }
            }
        }

        response->deleteLater();
    }

    return _exec(params);
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

HttpClient *HttpClient::instance()
{
	static HttpClient client;
	return &client;
}

HttpClient *HttpClient::createInstanceForAutoDelete()
{
	HttpClient* client = new HttpClient();
	client->m_bAutoDelete = true;
	return client;
}

HttpClient::HttpClient()
{
}

HttpRequest HttpClient::head(const QString &url)
{
    return HttpRequest(QNetworkAccessManager::HeadOperation, this).url(url);
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

#if (QT_VERSION < QT_VERSION_CHECK(5, 8, 0))
QNetworkReply *HttpClient::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
{
    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);
    QNetworkReply *reply = QNetworkAccessManager::sendCustomRequest(request, verb, buffer);
    buffer->setParent(reply);

    return reply;
}

QNetworkReply *HttpClient::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QHttpMultiPart *multiPart)
{
    if (verb == "PUT") {
        return QNetworkAccessManager::put(request, multiPart);
    }
    else if (verb == "POST") {
        return QNetworkAccessManager::post(request, multiPart);
    }
    else {
        qDebug() << "not support " << verb << "multi part.";
        return nullptr;
    }
}
#endif

HttpResponse::HttpResponse(HttpRequest::Params params, HttpRequest httpRequest)
    : QObject(params.reply),
      m_params(params),
      m_httpRequest(httpRequest),
      m_retriesRemaining(params.retryCount)
{
    int timeout          = params.timeoutMs;
    int isBlock          = params.isBlock;
    auto handleMap       = params.handleMap;
    QNetworkReply *reply = params.reply;

    new HttpResponseTimeout(this, timeout);

    if (reply) {
        connect(reply, SIGNAL(finished()),                         this, SLOT(onFinished()));
        connect(reply, SIGNAL(finished()),                         this, SLOT(onHandleHead()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));

        connect(reply, SIGNAL(downloadProgress(qint64, qint64)),   this, SLOT(onDownloadProgress(qint64, qint64)));
        connect(reply, SIGNAL(uploadProgress(qint64, qint64)),     this, SLOT(onUploadProgress(qint64, qint64)));

        connect(reply, SIGNAL(readyRead()),                        this, SLOT(onReadOnceReplyHeader()));
        connect(reply, SIGNAL(readyRead()),                        this, SLOT(onReadyRead()));
        connect(reply, SIGNAL(readyRead()),                        this, SLOT(onHandleHead()));

        connect(reply, SIGNAL(encrypted()),                        this, SLOT(onEncrypted()));
        connect(reply, SIGNAL(metaDataChanged()),                  this, SLOT(onMetaDataChanged()));

        connect(reply, SIGNAL(redirected(QUrl)),                   this, SLOT(onRedirected(QUrl)));
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)),        this, SLOT(onSslErrors(QList<QSslError>)));

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
        connect(reply, SIGNAL(redirectAllowed()),                  this, SLOT(onRedirectAllowed()));
    #endif

        connect(reply, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)), this, SLOT(onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)));

        connect(reply->manager(), SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(onAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    }

    // fixme: Too cumbersome
    for (auto each : handleMap.toStdMap()) {
        const HttpRequest::HandleType         &key   = each.first;
        const QList<QPair<QString, QVariant>> &value = each.second;

        for (auto iter : value) {
            const QVariant &lambda      = iter.second;
            const QString &lambdaString = iter.first;
            int ret = 0;

            if (key == HttpRequest::h_onFinished) {
                ret += HTTP_RESPONSE_CONNECT_X(this, finished, lambdaString, lambda, void);
                ret += HTTP_RESPONSE_CONNECT_X(this, finished, lambdaString, lambda, QByteArray);
                ret += HTTP_RESPONSE_CONNECT_X(this, finished, lambdaString, lambda, QVariantMap);
                ret += HTTP_RESPONSE_CONNECT_X(this, finished, lambdaString, lambda, QNetworkReply*);
            }
            else if (key == HttpRequest::h_onDownloadProgress) {
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadProgress, lambdaString, lambda, qint64, qint64);
            }
            else if (key == HttpRequest::h_onUploadProgress) {
                ret += HTTP_RESPONSE_CONNECT_X(this, uploadProgress, lambdaString, lambda, qint64, qint64);
            }
            else if (key == HttpRequest::h_onError) {
                ret += HTTP_RESPONSE_CONNECT_X(this, error, lambdaString, lambda, void);
                ret += HTTP_RESPONSE_CONNECT_X(this, error, lambdaString, lambda, QByteArray);
                ret += HTTP_RESPONSE_CONNECT_X(this, error, lambdaString, lambda, QNetworkReply*);
                ret += HTTP_RESPONSE_CONNECT_X(this, error, lambdaString, lambda, QNetworkReply::NetworkError);
            }
            else if (key == HttpRequest::h_onTimeout) {
                ret += HTTP_RESPONSE_CONNECT_X(this, timeout, lambdaString, lambda, QNetworkReply*);
                ret += HTTP_RESPONSE_CONNECT_X(this, timeout, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onReadyRead) {
                ret += HTTP_RESPONSE_CONNECT_X(this, readyRead, lambdaString, lambda, QNetworkReply*);
            }
            else if (key == HttpRequest::h_onDownloadFileSuccess) {
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileFinished, lambdaString, lambda, void);
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileFinished, lambdaString, lambda, QString);
            }
            else if (key == HttpRequest::h_onDownloadFileFailed) {
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileError, lambdaString, lambda, void);
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileError, lambdaString, lambda, QString);
            }
            else if (key == HttpRequest::h_onEncrypted) {
                ret += HTTP_RESPONSE_CONNECT_X(this, encrypted, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onMetaDataChanged) {
                ret += HTTP_RESPONSE_CONNECT_X(this, metaDataChanged, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onPreSharedKeyAuthenticationRequired) {
                ret += HTTP_RESPONSE_CONNECT_X(this, preSharedKeyAuthenticationRequired, lambdaString, lambda, QSslPreSharedKeyAuthenticator*);
            }
            else if (key == HttpRequest::h_onRedirectAllowed) {
                ret += HTTP_RESPONSE_CONNECT_X(this, redirectAllowed, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onRedirected) {
                ret += HTTP_RESPONSE_CONNECT_X(this, redirected, lambdaString, lambda, QUrl);
            }
            else if (key == HttpRequest::h_onSslErrors) {
                ret += HTTP_RESPONSE_CONNECT_X(this, sslErrors, lambdaString, lambda, QList<QSslError>);
            }
            else if (key == HttpRequest::h_onRetried) {
                ret += HTTP_RESPONSE_CONNECT_X(this, retried, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onRepeated) {
                ret += HTTP_RESPONSE_CONNECT_X(this, repeated, lambdaString, lambda, void);
            }
            else if (key == HttpRequest::h_onAuthenticationRequired) {
                ret += HTTP_RESPONSE_CONNECT_X(this, authenticationRequired, lambdaString, lambda, QAuthenticator*);
            }
            else if (key == HttpRequest::h_onAuthenticationRequireFailed) {
                ret += HTTP_RESPONSE_CONNECT_X(this, authenticationRequireFailed, lambdaString, lambda, void);
                ret += HTTP_RESPONSE_CONNECT_X(this, authenticationRequireFailed, lambdaString, lambda, QNetworkReply*);
            }
            else if (key == HttpRequest::h_onHead) {
                ret += HTTP_RESPONSE_CONNECT_X(this, head, lambdaString, lambda, QList<QNetworkReply::RawHeaderPair>);
                ret += HTTP_RESPONSE_CONNECT_X(this, head, lambdaString, lambda, QMap<QString, QString>);
            }
            else if (key == HttpRequest::h_onDownloadFileProgess) {
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileProgress, lambdaString, lambda, qint64, qint64);
            }
            else if (key == HttpRequest::h_onDownloadFileNameChanged) {
                ret += HTTP_RESPONSE_CONNECT_X(this, downloadFileNameChanged, lambdaString, lambda, QString);
            }
            else {
                // do nothing
                qWarning() << "Warning:" << key << "unsupported!";
            }

            if (ret == 0) {
                QString method = lambdaString;
                if (isMethod(qPrintable(method)))
                    method.remove(0, 1);

                qWarning() << "Warning:" << key << "method["<< method << "] is invalid!";
            }

        }
    }

    if (reply) {
        new HttpBlocker(reply, isBlock);
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
        emit downloadFileFinished();
        emit downloadFileFinished(m_downloadFile.fileName());

        m_downloadFile.close();
    }

    bool isAutoDelete = true;
    if (this->receivers(SIGNAL(finished(QNetworkReply*))) > 0) {
        emit finished(reply);
        isAutoDelete = false;
    }

    if (this->receivers(SIGNAL(finished())) > 0 ||
        this->receivers(SIGNAL(finished(QByteArray))) > 0 ||
        this->receivers(SIGNAL(finished(QVariantMap))) > 0)
    {
        QByteArray result = reply->readAll();
        emit finished();

        emit finished(result);

        QVariantMap resultMap = QJsonDocument::fromJson(result).object().toVariantMap();
        emit finished(resultMap);
    }

    if (--m_params.repeatCount > 0) {
        HttpRequest httpRequest = m_httpRequest;
        httpRequest.repeat(m_params.repeatCount)
                   .exec();
    }
    else {
        emit repeated();

		if( m_httpRequest.isAutoDeleteClient() )
			m_params.httpClient->deleteLater();
    }

    if (isAutoDelete) {
        reply->deleteLater();
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

    if (m_params.downloader.isEnabled) {
        QString error = QString("Url: %1 file: %2 error: %3")
                .arg(m_params.request.url().toString()) // fixme
                .arg(m_downloadFile.fileName())
                .arg(errorString);

        emit downloadFileError();
        emit downloadFileError(error);

        m_downloadFile.close();
    }

    bool isAutoDelete = true;
    if (this->receivers(SIGNAL(error(QNetworkReply*))) > 0) {
        emit this->error(reply);
        isAutoDelete = false;
    }

    emit this->error();
    emit this->error(error);
    emit this->error(errorString.toLocal8Bit());

    if (--m_params.repeatCount > 0) {
        HttpRequest httpRequest = m_httpRequest;
        httpRequest.repeat(m_params.repeatCount)
                   .exec();
    }
    else {
        emit repeated();
		if( m_httpRequest.isAutoDeleteClient() )
			m_params.httpClient->deleteLater();
    }

    if (isAutoDelete) {
        reply->deleteLater();
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

        bool isAutoDelete = true;
        if (this->receivers(SIGNAL(timeout(QNetworkReply*))) > 0) {
            emit this->timeout(reply);
            isAutoDelete = false;
        }

        if (this->receivers(SIGNAL(timeout())) > 0) {
            emit this->timeout();
        }

        if (isAutoDelete) {
            reply->deleteLater();
        }
    }
}

void HttpResponse::onReadyRead()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());

    if (m_params.downloader.isEnabled) {
        if (m_downloadFile.isOpen()){
            int size = m_downloadFile.write(reply->readAll());
            if (size == -1) {
                QString error = QString("Url: %1 %2 Write failed!")
                                .arg(m_params.request.url().toString())
                                .arg(m_downloadFile.fileName());
                emit downloadFileError();
                emit downloadFileError(error);
            }
            else {
                m_params.downloader.currentSize += size;
                emit downloadFileProgress(m_params.downloader.currentSize, m_params.downloader.totalSize);
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
    if (! m_params.downloader.isEnabled)
        return;

    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(onReadOnceReplyHeader()));

    QString fileName = m_params.downloader.fileName;
    m_downloadFile.setFileName(fileName);

    QIODevice::OpenMode mode = QIODevice::WriteOnly;
    if (m_params.downloader.isSupportBreakpointDownload &&
        m_params.downloader.enabledBreakpointDownload &&
        QFile::exists(fileName))
    {
        mode = QIODevice::Append;
    }

    if (!m_downloadFile.open(mode)) {
        QString error = QString("Url: %1 %2 Non-Writable")
                .arg(m_params.request.url().toString())
                .arg(m_downloadFile.fileName());
        emit downloadFileError();
        emit downloadFileError(error);
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

void HttpResponse::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    if (this->reply() != reply) {
        return;
    }

    m_authenticationCount++;

    bool isAuthenticationSuccessed = (m_authenticationCount >= 2);
    if (isAuthenticationSuccessed) {
        emit authenticationRequireFailed();
        emit authenticationRequireFailed(this->reply());
    }

    if (m_params.authenticationRequiredCount >= 0 &&
        m_authenticationCount > m_params.authenticationRequiredCount)
    {
        return;
    }

    if (m_params.authenticator.isNull()) {
        emit authenticationRequired(authenticator);
    }
    else {
        authenticator->setUser(m_params.authenticator.user());
        authenticator->setPassword(m_params.authenticator.password());
        // todo setOption....
    }
}

void HttpResponse::onHandleHead()
{
    if (m_isHandleHead) {
        return;
    }

    m_isHandleHead = true;

    QNetworkReply *reply = static_cast<QNetworkReply *>(this->parent());
    if (this->receivers(SIGNAL(head(QList<QNetworkReply::RawHeaderPair>))) ||
        this->receivers(SIGNAL(head(QMap<QString, QString>)))
       )
    {
        emit head(reply->rawHeaderPairs());
        QMap<QString, QString> map;
        foreach (auto each, reply->rawHeaderPairs()) {
            map[each.first] = each.second;
        }

        emit head(map);
    }
}

}

#define HTTPRESPONSE_DECLARE_METATYPE(...) \
    Q_DECLARE_METATYPE(std::function<void (__VA_ARGS__)>)

HTTPRESPONSE_DECLARE_METATYPE(void)
HTTPRESPONSE_DECLARE_METATYPE(QByteArray)
HTTPRESPONSE_DECLARE_METATYPE(QString)
HTTPRESPONSE_DECLARE_METATYPE(QVariantMap)
HTTPRESPONSE_DECLARE_METATYPE(QNetworkReply*)
HTTPRESPONSE_DECLARE_METATYPE(qint64, qint64)
HTTPRESPONSE_DECLARE_METATYPE(QNetworkReply::NetworkError)
HTTPRESPONSE_DECLARE_METATYPE(QSslPreSharedKeyAuthenticator*)
HTTPRESPONSE_DECLARE_METATYPE(QUrl)
HTTPRESPONSE_DECLARE_METATYPE(QList<QSslError>)
HTTPRESPONSE_DECLARE_METATYPE(QAuthenticator*)
HTTPRESPONSE_DECLARE_METATYPE(QList<QNetworkReply::RawHeaderPair>)
HTTPRESPONSE_DECLARE_METATYPE(QMap<QString, QString>)

#endif // QTHUB_COM_HTTPCLIENT_HPP
