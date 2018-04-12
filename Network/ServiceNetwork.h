#ifndef CPP_SERVICENETWORK_H
#define CPP_SERVICENETWORK_H

#include "Network.h"

class ServiceNetwork : public Network
{
    Q_OBJECT
public:
    ServiceNetwork();

    void update1(QNetworkReply* reply);
    void update2(QNetworkReply *reply);
};

#endif // CPP_SERVICENETWORK_H
