/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>

namespace AeaQt {

class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent,
                             const QHostAddress &host,
                             quint16 port);

    static bool send(const QByteArray &data,
                            const QHostAddress &address,
                            quint16 port);

   bool send(const QByteArray &data);

private:
    QHostAddress m_host;
    quint16 m_port;
};

}
#endif // UDPCLIENT_H
