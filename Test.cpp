#include "Test.h"

Test::Test()
{
}

void Test::httpTest()
{
    m_httpService.get("http://www.aeagean.com")
            .onResponse(this, SLOT(finish(QNetworkReply *)))
            .onResponse(this, SLOT(downloadProgress(qint64, qint64)))
            .onResponse(this, SLOT(error(QNetworkReply::NetworkError)))
            .onResponse(this, SLOT(error(QString)))
            .exec();
}

void Test::execute()
{
//    httpTest();
    otherTest();
}


#include <functional>

template<typename R = void, typename... Args>
class Fn {
public:
    Fn(std::function<R(Args...)> fun) : _fun(fun) {
    }

    R operator()(Args... args) {
        return _fun(args...);
    }
private:
    std::function<R(Args...) > _fun;
};

//Q_DECLARE_METATYPE(std::function<void ()>)
Q_DECLARE_METATYPE(void**)
void Test::otherTest()
{
    Fn<> sumFn([=]() { qDebug()<<"LLLL";});

    std::function<void ()> aa = sumFn;
    void **a = reinterpret_cast<void **>(&aa);
//    QVariant var = QVariant::fromValue(a);
    (reinterpret_cast<std::function<void ()>>(*a));
}

void Test::finish(QVariantMap result)
{
    qDebug()<<"reply: "<<result;
}

void Test::finish(QByteArray result)
{
    qDebug()<<"reply(QByteArray): "<<QString(result).simplified();
}

void Test::finish(QNetworkReply *reply)
{
    qDebug()<<"reply(type(QNetworkReply*): "<<reply->readAll();
}


void Test::finish(int value)
{
    qDebug()<<"reply(type(int): "<<value;
}

void Test::error(QNetworkReply::NetworkError result) {
    qDebug()<<"error: "<<result;
}

void Test::error(QString errorString)
{
    qDebug()<<"error: type(QString)"<<errorString;
}

void Test::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"bytesReceived: "<<bytesReceived;
    qDebug()<<"bytesTotal: "<<bytesTotal;
}
