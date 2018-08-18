#include "UdpClient.h"

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
