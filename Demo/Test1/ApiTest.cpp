/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
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
    m_service.get("http://mobilecdn.kugou.com/api/v3/search/song")
            .queryParam("format", "json")
            .queryParam("keyword", name)
            .queryParam("page", 1)
            .queryParam("pagesize", 3)
            .queryParam("showtype", 1)
            .onResopnse([this, name](QVariantMap result){
                QVariantMap data;
                QList<QVariant> infos;
                if (!result.isEmpty())
                    data = result.value("data").toMap();

                if (!data.isEmpty())
                    infos = data.value("info").toList();

                foreach (QVariant each, infos) {
                    m_service.get("http://m.kugou.com/app/i/getSongInfo.php")
                        .queryParam("cmd", "playInfo")
                        .queryParam("hash", each.toMap()["hash"])
                        .onResopnse([this, name](QVariantMap result){
                            QString url = result["url"].toString();
                            qDebug()<<"Get Url: "<<url;
                            m_service.get(url)
                                    .userAttribute(name)
                                    .onResopnse([this](QNetworkReply *result) {
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
    static HttpService http;
//    http.get("https://www.qt.io")
//             .onResopnse([](QByteArray result){ qDebug()<<"Result: "<<result; })
//             .onResopnse([](qint64 recv, qint64 total){ qDebug()<<"Total: "<<total<<"; Received: "<<recv; })
//             .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
//             .exec();//->networkReply()->abort();

    http.get("https://stackoverflow.com/questions/43707722/cant-run-qt-apps-version-qt-5-not-found")
             .onResponse(this, SLOT(finish(QByteArray)))
//             .onResponse(this, SLOT(downloadProgress(qint64,qint64)))
             .onError(this, SLOT(error(QString)))
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
