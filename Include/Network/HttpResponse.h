/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <QNetworkReply>
#include <QMultiMap>
#include <functional>

#define TYPE_TO_STRING(t) QString(#t).remove(QRegExp("\\s"))
#define NUMBER_TO_STRING(n)  QString::number(n)
#define N2S(n) NUMBER_TO_STRING(n)
#define T2S(t) TYPE_TO_STRING(t)

namespace AeaQt {

class HttpResponse : public QNetworkReply
{
    Q_OBJECT
public:
    /*
     * Support Reflex Method
     * default: AutoInfer
     * AutoInfer: Automatic derivation based on type
     */
    enum SupportMethod {
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

    explicit HttpResponse(QNetworkReply *parent, const QMultiMap<QString, QMap<QString, QVariant> > &slotsMap);

    virtual ~HttpResponse();

public slots:
    void abort();

private slots:
    void onFinished();
    void onError(QNetworkReply::NetworkError error);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    qint64 readData(char *data, qint64 maxlen);

    void slotsMapOperation(QMultiMap<QString, QMap<QString, QVariant> > &slotsMap);

signals:
    void finished(QNetworkReply *reply);
    void finished(QByteArray data);
    void finished(QVariantMap map);
    void error(QString errorString);
    void error(QNetworkReply::NetworkError error);
    void error(QString errorString, QNetworkReply *reply);
    void error(QNetworkReply::NetworkError error, QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    HttpResponse();

private:
   QMultiMap<QString, QMap<QString, QVariant> > m_slotsMap;
};

}

Q_DECLARE_METATYPE(std::function<void (QNetworkReply*)>)
Q_DECLARE_METATYPE(std::function<void (QByteArray)>)
Q_DECLARE_METATYPE(std::function<void (QVariantMap)>)

Q_DECLARE_METATYPE(std::function<void (QString)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError)>)
Q_DECLARE_METATYPE(std::function<void (QNetworkReply::NetworkError, QNetworkReply *)>)
Q_DECLARE_METATYPE(std::function<void (QString, QNetworkReply *)>)

Q_DECLARE_METATYPE(std::function<void (qint64, qint64)>)

#endif // HTTP_RESPONSE_H
