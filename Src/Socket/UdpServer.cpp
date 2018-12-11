/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#include "UdpServer.h"

using namespace AeaQt;

UdpServer::UdpServer(QObject *parent) : QObject(parent)
{
}

UdpServer::UdpServer(QObject *parent, const QHostAddress &address, quint16 port, QAbstractSocket::BindMode mode) :
    QObject(parent)
{
    m_udpServer = new QUdpSocket(this);
    m_udpServer->bind(address, port);
    connect(m_udpServer, SIGNAL(readyRead()), this, SLOT(receiver()));
}

void UdpServer::receiver()
{
    QByteArray result;
    while(m_udpServer->hasPendingDatagrams())
    {
        result.resize(m_udpServer->pendingDatagramSize());
        m_udpServer->readDatagram(result.data(), result.size());
        emit finish(result);
    }
}
