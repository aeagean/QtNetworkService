#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>
#include <QMultiMap>

class HttpRequest : public QNetworkReply
{
    Q_OBJECT
public:
    explicit HttpRequest(QNetworkReply *parent, QMap<QString, QMap<QString, const QObject *> > slotsMap);
    explicit HttpRequest(QNetworkReply *parent);

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
