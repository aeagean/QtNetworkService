/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#include "UdpClient.h"

using namespace AeaQt;

UdpClient::UdpClient(QObject *parent, const QHostAddress &host, quint16 port) :
    QObject(parent)
{
    m_host = host;
    m_port = port;
}

bool UdpClient::send(const QByteArray &writeData, const QHostAddress &host, quint16 port)
{
    return QUdpSocket().writeDatagram(writeData, host, port) != -1;
}

bool UdpClient::send(const QByteArray &data)
{
    return UdpClient::send(data, m_host, m_port);
}
