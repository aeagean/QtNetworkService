#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>
#include <QMap>

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
        onResponse_QNetworkReply_A_Pointer,    /* method: void function(QNetworkReply* reply) */
        onResponse_QByteArray,                 /* method: void function(QByteArray data) */
        onResponse_QVariantMap,                /* method: void function(QVariantMap map) */
        onDownloadProgress_qint64_qint64,      /* method: void function(qint64 bytesReceived, qint64 bytesTotal) */
        onError_QNetworkReply_To_NetworkError, /* method: void function(QNetworkReply::NetworkError error) */
        onError_QString                        /* method: void function(QString errorString) */
    };

    explicit HttpResponse(QNetworkReply *parent, const QMap<QString, QMap<QString, const QObject *> > &slotsMap);

    virtual ~HttpResponse();

public slots:
    void abort();

protected:
    qint64 readData(char *data, qint64 maxlen);
    void triggerSlot(const QObject *receiver, const char *receiverSlot);
    void slotsMapOperation(const QMap<QString, QMap<QString, const QObject *> > &slotsMap,
                           SupportMethod supportReflexMethod);

private:
    HttpResponse();
};


#endif // EMAQT_HTTP_REQUEST_H
