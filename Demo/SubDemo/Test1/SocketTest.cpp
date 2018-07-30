#include "SocketTest.h"

SocketTest::SocketTest(QObject *parent) : QObject(parent)
{
    UdpServer *udpServer = new UdpServer(parent, QHostAddress("127.0.0.1"), 8000);
    connect(udpServer, SIGNAL(finish(QByteArray)), this, SLOT(finish(QByteArray)));
    qDebug()<<"Starting listen.";
}

void SocketTest::finish(QByteArray result)
{
    qDebug()<<"Receive: "<<result;
}
