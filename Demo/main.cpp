/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qthub.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "HttpClient.hpp"
#include <QCoreApplication>
#include <QDebug>

using namespace AeaQt;

class Object : public QObject
{
    Q_OBJECT
public:
    Object()
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
              .timeout(1000) // 1s超时
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
        client.post("http://localhost:1234/post")
              .bodyWithFile("text_file", "helloworld.txt")
              .onSuccess(this, SLOT(onSuccess(QString)))
              .onFailed(this, SLOT(onFailed(QString)))
              .exec();
        // [6]
    }


public slots:
    // http成功返回
    void onSuccess(QString result)
    {
        qDebug() << "result: " << result.left(10);
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
    Object object;

#if 0
    HttpClient client;
    client.post("http://localhost:1234/post")
//    client.post("http://httpbin.org/post")
          .bodyWithFile("text_file", "helloworld.txt")
          .onSuccess([](QString result) { qDebug()<<"result: " << result.toLatin1(); })
          .onDownloadProgress([](qint64 bytesReceived, qint64 bytesTotal) {
              qDebug() << "lambda bytes received: " << bytesReceived
                       << "bytes total: " << bytesTotal;
           })
          .onFailed([](QString error) { qDebug()<<"error: " << error; })
          .exec();
#endif

    return a.exec();
}
