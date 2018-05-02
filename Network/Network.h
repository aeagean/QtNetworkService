#ifndef CPP_NETWORK_H
#define CPP_NETWORK_H

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSignalMapper>

#define NETWORK_NO_ERROR 200

class Network : public QNetworkAccessManager
{
    Q_OBJECT
public:
    Network();
    virtual ~Network();

    void get(QNetworkRequest request, const QObject* replyReceiver, const char *slot);
    void post(QNetworkRequest request, const QByteArray &data, const QObject* replyReceiver, const char *slot);

private slots:
    void serviceRequestFinished(QNetworkReply *reply);

private:
    bool bindingSignal(QNetworkRequest &request, const QObject* replyReceiver, const char *slot);
};

class NetworkRequest : public QObject
{
    Q_OBJECT
public:
    NetworkRequest(QObject *parent = NULL) :
        QObject(parent) { }

    void finished(QVariant var) {
        emit _finished(var);
    }

signals:
    void _finished(QVariant var);
};
#endif // CPP_NETWORK_H
