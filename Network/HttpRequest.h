#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>

class HttpRequest : public QNetworkReply
{
    Q_OBJECT
public:
    explicit HttpRequest(QNetworkReply* parent);
    virtual ~HttpRequest();

    void onResponse(const QObject *respReceiver, const char *respReceiverSlot);
    void onError(const QObject *errorReceiver, const char *slotErrorReceiver);

public slots:
    void abort();

private slots:
    void onFinished();
    void onErrored(QNetworkReply::NetworkError error);

protected:
    qint64 readData(char *data, qint64 maxlen);

private:
    HttpRequest();

signals:
    void finished(const QVariant result);
    void errored(const QVariant result);
};


#endif // EMAQT_HTTP_REQUEST_H
