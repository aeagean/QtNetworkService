# 示例
* (1) 简单示例
```cpp
/* 使用lambda特性 */
static HttpService http;
http.get("https://www.qt.io")
    .onResopnse([](QByteArray result){ qDebug()<<"Result: "<<result; })
    .onResopnse([](qint64 recv, qint64 total){ qDebug()<<"Total: "<<total<<"; Received: "<<recv; })
    .onError([](QString errorStr){ qDebug()<<"Error: "<<errorStr; })
    .exec();
```
```cpp
/* 使用Qt信号与槽特性 */
http.get("https://www.qt.io")
    .onResponse(this, SLOT(finish(QByteArray)))
    .onResponse(this, SLOT(downloadProgress(qint64,qint64)))
    .onError(this, SLOT(error(QString)))
    .exec();
```

* (2) 复杂示例
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

## 3.更多请关注微信公众号
<p align="center">
  <img src="http://www.qtbig.com/about/index/my_qrcode.jpg" alt="微信公众号:你才小学生">
  <p align="center"><em>你才小学生(nicaixiaoxuesheng)</em></p>
</p>

## 4.以下摘自公众号部分文章:
> Qt开源网络库[1]-介绍篇
<p align="center">
  <img src="https://github.com/aeagean/QtNetworkService/blob/master/Image/Qt%E5%BC%80%E6%BA%90%E7%BD%91%E7%BB%9C%E5%BA%93%5B1%5D-%E4%BB%8B%E7%BB%8D%E7%AF%87.png" alt="Qt开源网络库[1]-介绍篇">
</p>

---
> Qt开源网络库[2]-接口篇
<p align="center">
  <img src="https://github.com/aeagean/QtNetworkService/blob/master/Image/Qt%E5%BC%80%E6%BA%90%E7%BD%91%E7%BB%9C%E5%BA%93%5B2%5D-%E6%8E%A5%E5%8F%A3%E7%AF%87.png" alt="Qt开源网络库[2]-接口篇">
</p>
