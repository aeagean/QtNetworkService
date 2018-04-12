#ifndef CPP_NETWORK_H
#define CPP_NETWORK_H

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVariantMap>

class Network : public QObject
{
    Q_OBJECT
public:
    /* FNetwork => function network */
    typedef void (Network::*FNetwork)(QNetworkReply*);
    Network();
    ~Network();

    void get(const QString url);
    void post(const QString url, const QByteArray &data);

    QNetworkRequest &networkRequest();
    QMap<QString, FNetwork> &getFNetworkMap();

private slots:
    void serviceRequestFinished(QNetworkReply *reply);

private:
    QNetworkRequest m_networkRequest;
    QNetworkAccessManager m_networkAccessManager;
    QMap<QString, FNetwork> m_FNetworkMap;
};

#endif // CPP_NETWORK_H
