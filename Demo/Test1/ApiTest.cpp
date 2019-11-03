/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "ApiTest.h"
#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QUuid>

using namespace AeaQt;

ApiTest::ApiTest()
{
}

ApiTest::~ApiTest()
{
    qDebug()<<"ApiTest";
}

void ApiTest::downloadOneMusic(const QString &name)
{
    m_httpClient.get("http://mobilecdn.kugou.com/api/v3/search/song")
            .queryParam("format", "json")
            .queryParam("keyword", name)
            .queryParam("page", 1)
            .queryParam("pagesize", 3)
            .queryParam("showtype", 1)
            .onResponse([this, name](QVariantMap result){
                QVariantMap data;
                QList<QVariant> infos;
                if (!result.isEmpty())
                    data = result.value("data").toMap();

                if (!data.isEmpty())
                    infos = data.value("info").toList();

                foreach (QVariant each, infos) {
                    m_httpClient.get("http://m.kugou.com/app/i/getSongInfo.php")
                        .queryParam("cmd", "playInfo")
                        .queryParam("hash", each.toMap()["hash"])
                        .onResponse([this, name](QVariantMap result){
                            QString url = result["url"].toString();
                            qDebug()<<"Get Url: "<<url;
                            m_httpClient.get(url)
                                    .userAttribute(name)
                                    .onResponse([this](QNetworkReply *result) {
                                        QByteArray data = result->readAll();
                                        QString fileName = result->request().attribute(QNetworkRequest::User).toString() + QUuid::createUuid().toString();
                                        qDebug()<<"Saving: "<<fileName;
                                        saveFile(fileName, data);
                                     })
//                                    .onResopnse([](qint64 recv, qint64 total){ qDebug()<<"Total: "<<total<<"; Received: "<<recv; })
                                    .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
                                    .exec();
                         })
                        .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
                        .exec();
                }
            })
            .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
            .exec();
}

void ApiTest::exec()
{
//    m_httpClient.get("https://www.qt.io")
//             .onResopnse([](QByteArray result){ qDebug()<<"Result: "<<result; })
//             .onResopnse([](qint64 recv, qint64 total){ qDebug()<<"Total: "<<total<<"; Received: "<<recv; })
//             .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
//             .exec();

#if 0
    m_httpClient.post("http://127.0.0.1:8000/blog/")
                .body(R"({"3": 2})")
                .onResponse([](QByteArray result) {
                    qDebug() << "Result: " << result;
                 })
                .exec();
#endif

    m_httpClient.post("http://translate.google.cn/translate_a/single?client=gtx&dt=t&dj=1&ie=UTF-8&sl=auto&tl=zh_TW")
                .header("content-type", "application/json")
                .body(QJsonObject({{"q", "hello"}}))
                .onResponse(this, SLOT(finish(QByteArray)))
                .onResponse(this, SLOT(downloadProgress(qint64,qint64)))
                .onError(this, SLOT(error(QNetworkReply::NetworkError, QNetworkReply*)))
                .timeout(30*1000) // 30s
                .exec();
}

void ApiTest::saveFile(const QString &_fileName, QByteArray data)
{
    if (data.isEmpty()) {
         return;
     }

     QFileInfo fileInfo(_fileName);
     QString filePath = fileInfo.dir().path();
     QString fileName = fileInfo.fileName();
     QString filePathName = fileInfo.filePath();

     if (fileName == QString("") || filePath == QString()) {
         emit error("The file path saved is not correct! FilePathName: " + fileInfo.filePath());
         return;
     }

     QDir localDir;
     bool exist = localDir.exists(filePath);
     if(!exist) {
         bool ok = localDir.mkdir(filePath);
         if (!ok) {
             emit error("Create Dir: " + filePath + "failed!");
             return;
         }
     }

     QFile file(filePathName);
     file.open(QIODevice::WriteOnly);
     file.write(data);

     if (file.error() != QFile::NoError) {
         emit error(file.errorString());
         file.close();
         return;
     }

     file.close();
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
    qDebug()<<"Result: ";
    qDebug()<<result.left(100);
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
#if 0
    qDebug() << "Error content: " << reply->readAll();
#endif
    qDebug()<<"Error: "<<reply->error();
}

void ApiTest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<QString("Total: %1; Received: %2").arg(bytesTotal).arg(bytesReceived);
}
