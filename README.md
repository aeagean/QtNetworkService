# 示例
(1) 简单示例
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
* 以Qt子工程使用．再在其他子工程包含pri文件即可使用;
* 通过引用库的方式使用．

## 2.如何启用demo测试？
* 在QtNetworkService.pro文件中将"#CONFIG += QT_APP_MODE"这一行的#去除即可转为可执行文件形式，在Demo目录的main.cpp为主执行文件，如需要测试接口编辑便可。

## 3.扫码关注，第一时间获取推送
<p align="center">
  <img src="http://www.qtbig.com/about/index/my_qrcode.jpg" alt="微信公众号:Qt君">
  <p align="center"><em>Qt君</em></p>
</p>
