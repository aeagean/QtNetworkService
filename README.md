# 示例
1. 使用信号槽的方式实现成功与失败的事件处理
```cpp
static HttpClient client;
client.get("https://qthub.com")
      .onSuccess(this, SLOT(onSuccess(QString)))
      .onFailed(this, SLOT(onFailed(QString)))
      .exec(); // 执行Http操作
```

2. 使用匿名函数的方式实现成功与失败的事件处理
```cpp
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result:"<<result.left(100); })
      .onFailed([](QString error) { qDebug()<<"error:"<<error; })
      .exec();
```

3. 以信号槽的方式获取下载进度
```cpp
client.get("https://qthub.com")
      .onSuccess(this, SLOT(onSuccess(QString)))
      .onDownloadProgress(this, SLOT(onDownloadProgress(qint64, qint64)))
      .onFailed(this, SLOT(onFailed(QString)))
      .exec();
```

4. 以匿名函数的方式获取下载进度
```cpp
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result: " << result.left(10); })
      .onDownloadProgress([](qint64 bytesReceived, qint64 bytesTotal) {
          qDebug() << "lambda bytes received: " << bytesReceived
                   << "bytes total: " << bytesTotal;
       })
      .onFailed([](QString error) { qDebug()<<"error: " << error; })
      .exec();
```

5. 自定义超时时间和超时处理
* timeout(ms)是设置超时时间，单位为毫秒(ms)。
* onTimeout为超时回调，当超时事件触发，自动调用onTimeout回调。
```cpp
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result:"<<result.left(100); })
      .onFailed([](QString error) { qDebug()<<"error:"<<error; })
      .onTimeout([](QNetworkReply *) { qDebug()<<"timeout"; })
      .timeout(1000) // 1s超时
      .exec();
```
* 使用lambda特性
```cpp
static HttpClient http;
http.post("https://example.com")
    .header("content-type", "application/json")
    .queryParam("key", "Hello world!")
    .body(R"({"user": "test"})")
    .onResponse([](QByteArray result) { /* 接收数据 */
        qDebug() << "Result: " << result;
     })
    .onResponse([](qint64 recv, qint64 total) { /* 接收进度 */
        qDebug() << "Total: " << total << "; Received: " << recv;
     })
    .onError([](QString errorStr) { /* 错误处理 */
        qDebug()<<"Error: "<<errorStr;
     })
    .timeout(30 * 1000) /* 超时操作(30s) */
    .block() /* 阻塞操作 */
    .exec();
```

* 使用Qt信号与槽特性
```cpp
http.post("https://example.com")
    .header("content-type", "application/json")
    .queryParam("key", "Hello world!")
    .body(R"({"user": "test"})")
    .onResponse(this, SLOT(finish(QByteArray)))
    .onResponse(this, SLOT(downloadProgress(qint64, qint64)))
    .onError(this, SLOT(error(QString)))
    .timeout(30 * 1000) /* 超时操作(30s) */
    .block() /* 阻塞操作 */
    .exec();
```

(2) 复杂示例
```cpp
/* 获取音乐url功能，请求嵌套请求 */
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
```
## 1.如何使用？
* 在其工程包含src目录的HttpClient.hpp文件即可;

## 2.如何启用demo测试？
* 在QtNetworkService.pro文件中将"#DEFINES += QT_APP_MODE"这一行的#去除即可转为可执行文件形式，在Demo目录的main.cpp为主执行文件，如需要测试接口编辑便可。

## 3.扫码关注，第一时间获取推送
<p align="center">
  <img src="http://www.qtbig.com/about/index/my_qrcode.jpg" alt="微信公众号:Qt君">
  <p align="center"><em>Qt君</em></p>
</p>
