/**********************************************************
 * Author: Qt君
 * 微信公众号: Qt君
 * Website: qthub.com
 * Email:  2088201923@qq.com
 * QQ交流群: 732271126
 * Source Code: https://github.com/aeagean/QtNetworkService
 * LISCENSE: MIT
**********************************************************/
#include "HttpClient.h"
#include <QCoreApplication>
#include <QDebug>

using namespace AeaQt;

class Object : public QObject
{
    Q_OBJECT
public:
    Object()
    {
    }

    void exec()
    {
        static HttpClient client;
        // [0] 使用信号槽方式实现简单的监听成功与失败事件处理
        client.get("https://qthub.com")
              .onSuccess(this, SLOT(onSuccess(QString)))
              .onFailed(this, SLOT(onFailed(QString)))
              .exec();
        // [0]

        // [1] 使用lambda方式实现简单的监听成功与失败的事件处理
        client.get("https://qthub.com")
              .onSuccess([](QString result) { qDebug()<<"result: " << result.left(10); })
              .onFailed([](QString error) { qDebug()<<"error: " << error; })
              .exec();
        // [1]

        // [2] 使用信号槽获取下载进度
        client.get("https://qthub.com")
              .onSuccess(this, SLOT(onSuccess(QString)))
              .onDownloadProgress(this, SLOT(onDownloadProgress(qint64, qint64)))
              .onFailed(this, SLOT(onFailed(QString)))
              .exec();
        // [2]

        // [3] lambda获取下载进度
        client.get("https://qthub.com")
              .onSuccess([](QString result) { qDebug()<<"result: " << result.left(10); })
              .onDownloadProgress([](qint64 bytesReceived, qint64 bytesTotal) {
                  qDebug() << "lambda bytes received: " << bytesReceived
                           << "bytes total: " << bytesTotal;
               })
              .onFailed([](QString error) { qDebug()<<"error: " << error; })
              .exec();
        // [3]

        // [4] 自定义超时处理
        client.get("https://qthub.com")
              .onSuccess([](QString result) { qDebug()<<"result: " << result.left(10); })
              .onDownloadProgress([](qint64 bytesReceived, qint64 bytesTotal) {
                  qDebug() << "lambda bytes received: " << bytesReceived
                           << "bytes total: " << bytesTotal;
               })
              .onFailed([](QString error) { qDebug()<<"error: " << error; })
              .onTimeout([](QNetworkReply *) { qDebug()<<"timeout"; })
              .timeout(1) // 1s超时, => timeoutMs(1000)
              .exec();
        // [4]

        // [5] 使用body
        QVariantMap map;
        map["&^="] = "1234";
        client.post("http://localhost:1234/post")
              .body(map)
              .onSuccess(this, SLOT(onSuccess(QString)))
              .onFailed(this, SLOT(onFailed(QString)))
              .exec();
        // [5]

        // [6] post文件
        client.post("http://httpbin.org/post")
              .bodyWithFile("text_file", "helloworld.txt")
              .bodyWithFile("image_file", "qt.jpg")
              .onUploadProgress([](qint64 bytesSent, qint64 bytesTotal) {
                  qDebug() << "lambda bytes sent: " << bytesSent
                           << "bytes total: " << bytesTotal;
               })
              .onSuccess([](QString result) { qDebug()<<"result: " << result.left(100); })
              .onFailed([](QString error) { qDebug()<<"error: " << error; })
              .exec();
        // [6]

        // [7] readyRead测试
        client.get("https://hub.fastgit.org/aeagean/QtNetworkService/archive/refs/heads/master.zip")
              .onReadyRead([](QNetworkReply *reply) { qDebug()<< "readyRead: "<<reply->readAll().size(); })
              .onSuccess([](QString result) { qDebug()<<"result: success"<<result; })
              .onFailed([](QString error) { qDebug()<<"error: "<<error; })
              .exec();
        // [7]

        // [8] 上传文件
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QString contentType = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());

        QFile *file = new QFile("demo.txt");
        file->open(QIODevice::ReadOnly);
        file->setParent(multiPart);

        QString dispositionHeader = QString("form-data; name=\"%1\";filename=\"%2\"")
                .arg("text_file")
                .arg(file->fileName());

        QHttpPart part;
        part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
        part.setHeader(QNetworkRequest::ContentDispositionHeader, dispositionHeader);
        part.setBodyDevice(file);

        multiPart->append(part);

        client.post("http://httpbin.org/post")
              .header("content-type", contentType)
              .body(multiPart)
              .onSuccess([](QString result){})
              .onFailed([](QString error){})
              .exec();
        // [8]

        // [9] 携带特定的用户数据到响应回调函数
        client.get("http://httpbin.org/get")
              .userAttribute("Hello world!")
              .onSuccess([](QNetworkReply *reply) {
                    QVariant value = reply->request().attribute(QNetworkRequest::User);
                    qDebug()<< value.toString();
               })
              .onFailed([](QString error){ qDebug()<<error; })
              .exec();

        // [9]

        // [10] 下载文件
        client.get("https://hub.fastgit.org/aeagean/QtNetworkService/archive/refs/heads/master.zip")
              .download()
              .onDownloadProgress([](qint64 bytesReceived, qint64 bytesTotal) {
                  qDebug() << "bytes received: " << bytesReceived
                           << "bytes total: " << bytesTotal;
               })
              .onDownloadSuccess([](QString fileName) { qDebug()<<"download success: "<<fileName; })
              .onDownloadFailed([](QString error) { qDebug()<<"download failed: "<<error; })
              .exec();
        // [10]

        // [x] 失败重试
        client.get("xxx://httpbin.org/get")
              .retry(2) // 失败重试的次数
              .onRetried([](){qDebug()<<"retried!";}) // 失败重试操作完成后的回调
              .onSuccess([](QString result){qDebug()<<result;})
              .onFailed([](QString err){qDebug()<<err;})
              .exec();
        // [x]

        // [x] 重复请求
        client.get("https://httpbin.org/get")
              .repeat(3) // 总共重复请求的次数
              .onRepeated([](){qDebug()<<"repeated!";}) // 重复请求操作完成后的回调
              .onSuccess([](QString result){qDebug()<<result;})
              .onFailed([](QString err){qDebug()<<err;})
              .exec();
        // [x]

        // [x] 同步操作,适用于一些顺序队列的请求
       client.get("https://httpbin.org/get")
             .onSuccess([](QString result){qDebug()<<result;})
             .onFailed([](QString err){qDebug()<<err;})
             .sync() // 同步操作
             .exec();
        // 或
       client.get("https://httpbin.org/get")
             .onSuccess([](QString result){qDebug()<<result;})
             .onFailed([](QString err){qDebug()<<err;})
             .block() // 同步操作
             .exec();
        // [x]
    }

public slots:
    // http成功返回
    void onSuccess(QString result)
    {
        qDebug() << "result: " << result.left(10);
    }

    void onSuccess(QNetworkReply *result)
    {
        qDebug() << "result: " << result->readAll();
    }

    // http失败返回
    void onFailed(QString errorString)
    {
        qDebug() << "error string: " << errorString;
    }

    // http下载进度
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        qDebug() << "signal-slot bytes received: " << bytesReceived
                 << "bytes total: " << bytesTotal;
    }

    void onTimeout()
    {
        qDebug() << "timeout" << __LINE__;
    }

    void onTimeout(QNetworkReply *reply)
    {
        qDebug() << "timeout" << __LINE__;
        reply->deleteLater();
    }

private:
    HttpClient m_httpClient;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 1
    Object object;
    object.exec();
#else
    Object object;
    HttpClient client;
    client.get("https://httpbin.org/get")
          .onSuccess([](QString result){qDebug()<<result;})
          .onSuccess(&object, SLOT(onSuccess(QString)))
          .onFailed([](QString err){qDebug()<<err;})
//          .timeout(1)
//            .onTimeout([](){qDebug()<<__LINE__;})
          .sync() // 同步操作
          .exec();

#endif

    return a.exec();
}
