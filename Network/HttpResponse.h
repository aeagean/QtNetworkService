#ifndef EMAQT_HTTP_REQUEST_H
#define EMAQT_HTTP_REQUEST_H

#include <QNetworkReply>
#include <QMap>

class HttpResponse : public QNetworkReply
{
    Q_OBJECT
public:
    enum SupportReflexMethod {
        onResponseMethod,
        onErrorMethod
    };

    explicit HttpResponse(QNetworkReply *parent, const QMap<QString, QMap<QString, const QObject *> > &slotsMap);

    virtual ~HttpResponse();

public slots:
    void abort();

protected:
    qint64 readData(char *data, qint64 maxlen);
    void triggerSlot(const QObject *receiver, const char *receiverSlot);
    void slotsMapOperation(const QMap<QString, QMap<QString, const QObject *> > &slotsMap,
                           SupportReflexMethod supportReflexMethod);

private:
    HttpResponse();
};


#endif // EMAQT_HTTP_REQUEST_H
