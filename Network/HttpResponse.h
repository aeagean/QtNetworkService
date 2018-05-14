#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>
#include <QMultiMap>

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
        onError_QString                        /* method: void function(QString errorString); Is_AutoInfer: true */
    };

    explicit HttpResponse(QNetworkReply *parent, const QMultiMap<QString, QMap<QString, const QObject *> > &slotsMap);

    explicit HttpResponse(QNetworkReply *parent) : QNetworkReply(parent) {}

    virtual ~HttpResponse();

public slots:
    void abort();

private slots:
    void onFinished();
    void onError();

protected:
    qint64 readData(char *data, qint64 maxlen);

    void slotsMapOperation(const QMultiMap<QString, QMap<QString, const QObject *> > &slotsMap);

signals:
    void finished(QNetworkReply *reply);
    void finished(QByteArray data);
    void finished(QVariantMap map);
    void error(QString errorString);

private:
    HttpResponse();

private:
   QMultiMap<QString, QMap<QString, const QObject *> > m_slotsMap;
};


#endif // EMAQT_HTTP_REQUEST_H
