/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#include "ApiTest.h"
#include <QDebug>
#include <QTimer>

using namespace AeaQt;

ApiTest::ApiTest()
{

}

ApiTest::~ApiTest()
{
    qDebug()<<"ApiTest";
}

void ApiTest::exec()
{
//    m_service.get("https://www.qt.io")
//             .onResopnse([](QByteArray result){ qDebug()<<"Result: "<<result; })
//             .onResopnse([](qint64 recv, qint64 total){ qDebug()<<"Total: "<<total<<"; Received: "<<recv; })
//             .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
//             .exec();

//    m_service.get("https://www.qt.io")
//             .onResponse(this, SLOT(finish(QByteArray)))
//             .onResponse(this, SLOT(downloadProgress(qint64,qint64)))
//             .onError(this, SLOT(error(QString)))
//             .exec();

    static HttpService http;
    http.get("http://mobilecdn.kugou.com/api/v3/search/song")
        .queryParam("format", "json")
        .queryParam("keyword", "稻香")
        .queryParam("page", 1)
        .queryParam("pagesize", 3)
        .queryParam("showtype", 1)
        .onResopnse([](QVariantMap result){
            QVariantMap data;
            QList<QVariant> infos;
            if (!result.isEmpty())
                data = result.value("data").toMap();

            if (!data.isEmpty())
                infos = data.value("info").toList();

            static HttpService http;
            foreach (QVariant each, infos) {
                http.get("http://m.kugou.com/app/i/getSongInfo.php")
                    .queryParam("cmd", "playInfo")
                    .queryParam("hash", each.toMap()["hash"])
                    .onResopnse([](QVariantMap result){
                        qDebug()<<"mp3: "<<result["url"].toString();
                     })
                    .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
                    .exec();
            }
        })
        .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
        .exec();
}

void ApiTest::finish(QVariantMap result)
{
    qDebug()<<"Result: "<<result;
}

void ApiTest::finish(QNetworkReply *reply)
{
    qDebug()<<"Result: "<<reply->readLine();
}

void ApiTest::finish(QByteArray result)
{
    qDebug()<<"Result: "<<result;
}

void ApiTest::error(QString errorString)
{
    qDebug()<<"Error: "<<errorString;
}

void ApiTest::error(QString errorString, QNetworkReply *reply)
{
    qDebug()<<"Error: "<<reply->error();
}

void ApiTest::error(QNetworkReply::NetworkError error)
{
    qDebug()<<"Error: "<<error;
}

void ApiTest::error(QNetworkReply::NetworkError error, QNetworkReply *reply)
{
    qDebug()<<"Error: "<<reply->error();
}

void ApiTest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<QString("Total: %1; Received: %2").arg(bytesTotal).arg(bytesReceived);
}
