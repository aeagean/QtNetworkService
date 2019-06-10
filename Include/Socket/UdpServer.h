/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>

namespace AeaQt {

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr,
                       const QHostAddress &address = QHostAddress(QHostAddress::Any),
                       quint16 port = 0,
                       QUdpSocket::BindMode mode = QUdpSocket::DefaultForPlatform);

signals:
    void finish(QByteArray result);

public slots:
    void receiver();

private:
    explicit UdpServer(QObject *parent = nullptr);

private:
    QUdpSocket *m_udpServer;
};

}
#endif // UDPSERVER_H
