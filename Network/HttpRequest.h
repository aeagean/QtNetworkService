#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>

class HttpRequest : public QNetworkReply
{
    Q_OBJECT
public:
    explicit HttpRequest(QNetworkReply* parent,
                         const QObject *respReceiver, const char *respReceiverSlot,
                         const QObject *errorReceiver, const char *errorReceiverSlot);
    virtual ~HttpRequest();

public slots:
    void abort();

protected:
    qint64 readData(char *data, qint64 maxlen);
    void initRequest(const QObject *receiver, const char *receiverSlot);

private:
    HttpRequest();
};


#endif // EMAQT_HTTP_REQUEST_H
