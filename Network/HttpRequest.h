#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include "HttpServiceMethod.h"

#include <QNetworkReply>
#include <QMultiMap>

class HttpRequest : public QNetworkReply
{
    Q_OBJECT
public:
    explicit HttpRequest(QNetworkReply *parent, const QMap<QString, QMap<QString, const QObject *> > &slotsMap);

    virtual ~HttpRequest();

public slots:
    void abort();

protected:
    qint64 readData(char *data, qint64 maxlen);
    void initRequest(const QObject *receiver, const char *receiverSlot);
    void slotsMapOperation(const QMap<QString, QMap<QString, const QObject *> > &slotsMap,
                           HttpServiceMethod::SupportReflexMethod supportReflexMethod);

private:
    HttpRequest();
};


#endif // EMAQT_HTTP_REQUEST_H
