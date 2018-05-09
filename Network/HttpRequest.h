#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>

class HttpRequest : public QNetworkReply
{
    Q_OBJECT
public:
    explicit HttpRequest(QNetworkReply* parent,
                         const QObject *respReceiver, const char *respReceiverSlot,
                         const QObject *errorReceiver, const char *ErrorReceiverSlot);
    virtual ~HttpRequest();

public slots:
    void abort();

protected:
    qint64 readData(char *data, qint64 maxlen);

private:
    HttpRequest();
};


#endif // EMAQT_HTTP_REQUEST_H
