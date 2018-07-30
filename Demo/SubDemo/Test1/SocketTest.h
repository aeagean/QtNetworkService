#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include "UdpServer.h"
#include <QObject>

class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = nullptr);

signals:

public slots:
    void finish(QByteArray result);
};

#endif // SOCKETTEST_H
