#ifndef CPP_SERVICENETWORK_H
#define CPP_SERVICENETWORK_H

#include "Network.h"

class ServiceNetwork : public Network
{
    Q_OBJECT
public:
    ServiceNetwork();

private slots:
    void update(QVariant data);
};

#endif // CPP_SERVICENETWORK_H
