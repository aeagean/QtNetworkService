一个简单的使用例子
```cpp
HttpClient client;
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result:"<<result; })
      .onFailed([](QString err) { qDebug()<<"error:"<<err; })
      .exec();
```

# 1. 如何使用？
* 采用header-only的方式实现。只需在你的工程中包含 src 目录的 HttpClient.h 文件即可。

# 2. 使用文档

## 2.1 使用信号槽的方式实现成功与失败的事件处理
### 接口:
* Http请求返回成功的信号槽绑定
```cpp
HttpRequest &onSuccess(const QObject *receiver, const char *method);
HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);
```

* Http请求返回失败的信号槽绑定
```cpp
HttpRequest &onFailed(const QObject *receiver, const char *method);
HttpRequest &onFailed(std::function<void (QString)> lambda);
HttpRequest &onFailed(std::function<void (QNetworkReply::NetworkError)> lambda);
HttpRequest &onFailed(std::function<void (QNetworkReply*)> lambda);
```

### 例子:
```cpp
static HttpClient client;
client.get("https://qthub.com")
      .onSuccess(this, SLOT(onSuccess(QString)))
      .onFailed(this, SLOT(onFailed(QString)))
      .exec(); // 执行Http操作
```

## 2.2 使用匿名函数的方式实现成功与失败的事件处理
### 接口:
* Http请求返回成功的回调事件
```cpp
HttpRequest &onSuccess(std::function<void (QNetworkReply*)> lambda);
HttpRequest &onSuccess(std::function<void (QVariantMap)> lambda);
HttpRequest &onSuccess(std::function<void (QByteArray)> lambda);
```

* Http请求返回失败的回调事件
```cpp
HttpRequest &onFailed(std::function<void (QString)> lambda);
HttpRequest &onFailed(std::function<void (QNetworkReply::NetworkError)> lambda);
HttpRequest &onFailed(std::function<void (QNetworkReply*)> lambda);
```

### 例子：
```cpp
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result:"<<result.left(100); })
      .onFailed([](QString error) { qDebug()<<"error:"<<error; })
      .exec();
```

## 2.3 以信号槽的方式获取下载进度
### 接口:
```cpp
HttpRequest &onDownloadProgress(const QObject *receiver, const char *method);
```

### 例子:
```cpp
client.get("https://qthub.com")
      .onSuccess(this, SLOT(onSuccess(QString)))
      .onDownloadProgress(this, SLOT(onDownloadProgress(qint64, qint64)))
      .onFailed(this, SLOT(onFailed(QString)))
      .exec();
```

## 2.4 以匿名函数的方式获取下载进度

### 接口:
```cpp
HttpRequest &onDownloadProgress(std::function<void (qint64, qint64)> lambda);
```

### 例子:
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

## 2.5 post 上传文件并获取上传进度

### 接口:
```cpp
HttpRequest &onUploadProgress(const QObject *receiver, const char *method);
HttpRequest &onUploadProgress(std::function<void (qint64, qint64)> lambda);
```

### 例子:
```cpp
client.post("http://httpbin.org/post")
      .bodyWithFile("text_file", "helloworld.txt")
      .onUploadProgress([](qint64 bytesSent, qint64 bytesTotal) {
          qDebug() << "lambda bytes sent: " << bytesSent
                   << "bytes total: " << bytesTotal;
       })
      .onSuccess([](QString result) { qDebug()<<"result: " << result.left(100); })
      .onFailed([](QString error) { qDebug()<<"error: " << error; })
      .exec();
```

## 2.6 自定义超时时间和超时处理

- timeout(ms)是设置超时时间，单位为毫秒(ms)。
- onTimeout 为超时回调，当超时事件触发，自动调用 onTimeout 回调。

### 接口:
* 设置超时时间
```cpp
HttpRequest &timeout(const int &msec = -1);
```

* 设置超时的回调函数
```cpp
HttpRequest &onTimeout(const QObject *receiver, const char *method);
HttpRequest &onTimeout(std::function<void (QNetworkReply*)> lambda);
HttpRequest &onTimeout(std::function<void ()> lambda);
```

### 例子:
```cpp
client.get("https://qthub.com")
      .onSuccess([](QString result) { qDebug()<<"result:"<<result.left(100); })
      .onFailed([](QString error) { qDebug()<<"error:"<<error; })
      .onTimeout([](QNetworkReply *) { qDebug()<<"timeout"; }) // 超时处理
      .timeout(1000) // 1s超时
      .exec();
```


## 2.7 由于 HttpClient 是异步实现，我们需要同步时可以这样做

### 接口:
```cpp
HttpRequest &block();
```

### 例子:
```cpp
client.get("https://qthub.com")
      .onSuccess(this, SLOT(onSuccess(QString)))
      .onFailed(this, SLOT(onFailed(QString)))
      .block() // 阻塞同步操作
      .exec(); // 执行Http操作
```

## 2.8 添加 header

### 接口:

```cpp
HttpRequest &header(const QString &key, const QVariant &value);
HttpRequest &headers(const QMap<QString, QVariant> &headers);
```

### 例子:

```cpp
client.post("https://example.com")
      .header("content-type", "application/json")
      .queryParam("key", "Hello world!")
      .body(R"({"user": "test"})")
      .onSuccess([](QString result){})
      .onFailed([](QString error){})
      .exec();
```

## 2.9 添加 params

### 接口:

```cpp
HttpRequest &queryParam(const QString &key, const QVariant &value);
HttpRequest &queryParams(const QMap<QString, QVariant> &params);
```

### 例子:

```cpp
client.get("https://example.com")
      .queryParam("key1", "value1")
      .queryParam("key2", "value2")
      .queryParam("key3", "value3")
      .onSuccess([](QString result){})
      .onFailed([](QString error){})
      .exec();
```

上面代码等同于:

```cpp
client.get("https://example.com?key1=value1&key2=value2&key3=value3")
      .onSuccess([](QString result){})
      .onFailed([](QString error){})
```

## 2.10 添加 body

### 接口:

- 原始数据

```cpp
HttpRequest &body(const QByteArray &raw);
HttpRequest &bodyWithRaw(const QByteArray &raw);
```

- json 数据

```cpp
HttpRequest &body(const QJsonObject &json);
HttpRequest &bodyWithJson(const QJsonObject &json);
```

- 表单数据

```cpp
HttpRequest &body(const QVariantMap &formUrlencodedMap);
HttpRequest &bodyWithFormUrlencoded(const QVariantMap &keyValueMap);
```

- 混合消息

```cpp
HttpRequest &body(QHttpMultiPart *multiPart);
HttpRequest &bodyWithMultiPart(QHttpMultiPart *multiPart);
```

- 文件消息

```cpp
HttpRequest &body(const QString &key, const QString &file);
HttpRequest &bodyWithFile(const QString &key, const QString &file);
HttpRequest &bodyWithFile(const QMap<QString/*key*/, QString/*file*/> &fileMap);
```

### 例子:

#### 发送原始数据

```cpp
client.post("http://httpbin.org/post")
      .body("hello world")
      .onSuccess([](QString result){qDebug()<<result;})
      .onFailed([](QString error){qDebug()<<error;})
      .exec();
```

#### 发送 json 数据

```cpp
QJsonObject json
{
    {"property1", 1},
    {"property2", 2}
};

client.post("http://httpbin.org/post")
      .body(json)
      .onSuccess([](QString result){qDebug()<<result;})
      .onFailed([](QString error){qDebug()<<error;})
      .exec();
```

#### 发送表单数据

```cpp
QVariantMap map
{
    {"property1", 1},
    {"property2", 2}
};

client.post("http://httpbin.org/post")
      .body(map)
      .onSuccess([](QString result){qDebug()<<result;})
      .onFailed([](QString error){qDebug()<<error;})
      .exec();
```

#### 发送混合消息

```cpp
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

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

    QString contentType = QString("multipart/form-data;boundary=%1").arg(multiPart->boundary().data());

HttpClient client;
client.post("http://httpbin.org/post")
      .header("content-type", contentType)
      .body(multiPart)
      .onSuccess([](QString result){ qDebug()<<result.left(1000); })
      .onFailed([](QString error){ qDebug()<<error; })
      .exec();
```

#### 发送文件

```cpp
HttpClient client;
client.post("http://httpbin.org/post")
      .body("text_file", "demo.txt")
      .body("image_file", "demo.jpg")
      .onSuccess([](QString result){ qDebug()<<result.left(1000); })
      .onFailed([](QString error){ qDebug()<<error; })
      .exec();
```

## 2.11 携带特定的用户数据到响应回调函数

### 接口:

```cpp
HttpRequest &userAttribute(const QVariant &value);
```

### 例子:

```cpp
client.get("http://httpbin.org/get")
      .userAttribute("Hello world!")
      .onSuccess([](QNetworkReply *reply) {
            QVariant value = reply->request().attribute(QNetworkRequest::User);
            qDebug()<< value.toString();
       })
      .onFailed([](QString error){ qDebug()<<error; })
      .exec();
```

## 2.12 下载文件
### 接口:
1. 设置下载操作
&emsp;&emsp;保存为默认的文件名，会从请求头去拿文件名字，如果请求头没有则为链接最后的文本内容。
```cpp
HttpRequest &download();
```

&emsp;&emsp;指定保存的文件名字,可包含路径。
```cpp
HttpRequest &download(const QString &file);
```

2. 下载成功或失败的响应回调/信号槽
```cpp
HttpRequest &onDownloadSuccess(const QObject *receiver, const char *method);
HttpRequest &onDownloadSuccess(std::function<void ()> lambda);
HttpRequest &onDownloadSuccess(std::function<void (QString)> lambda);

HttpRequest &onDownloadFailed(const QObject *receiver, const char *method);
HttpRequest &onDownloadFailed(std::function<void ()> lambda);
HttpRequest &onDownloadFailed(std::function<void (QString)> lambda);
```

3. 断点续传下载
&emsp;&emsp;默认开启断点续传下载功能，即当程序异常退出时，再次运行会接着从已下载的位置下载。
&emsp;&emsp;需要注意的是，如果服务器不支持断点续传功能，则每次下载都是从头开始下载。

 参数 | 解释
 --- | ---
 enabled | 开启/关闭断点续传下载

```cpp
HttpRequest &enabledBreakpointDownload(bool enabled = true);
```


4. 文件下载进度的响应回调/信号槽
&emsp;&emsp;回调传递的参数分别是已保存的文件字节数和文件总字节大小。
```cpp
HttpRequest &onFileDownloadProgress(const QObject *receiver, const char *method);
HttpRequest &onFileDownloadProgress(std::function<void (qint64, qint64)> lambda);
```

### 例子:
1. 简单下载文件
```cpp
client.get("https://hub.fastgit.org/aeagean/QtNetworkService/archive/refs/heads/master.zip")
      .download("QtNetworkService.zip")
      .onDownloadSuccess([](QString fileName) {
            qDebug() << "Download completed: " << fileName;
       })
      .onDownloadFailed([](QString error) {
            qDebug() << "Download failed: " << error;
       })
      .exec();
```

2. 断点续传下载
```cpp
client.get("http://mirrors.tuna.tsinghua.edu.cn/qt/archive/qt/6.0/6.0.3/single/qt-everywhere-src-6.0.3.tar.xz")
      .download() // 启用自动设置文件名字 => qt-everywhere-src-6.0.3.tar.xz
      .enabledBreakpointDownload() // 启用断点续传下载
      .onFileDownloadProgress([](qint64 recv, qint64 total) {
            // 获取文件下载进度
            qDebug().nospace() << (100 * qreal(recv)/total) << "%";
       })
      .onDownloadSuccess([](QString fileName) {
            qDebug() << "Download completed: " << fileName;
       })
      .onDownloadFailed([](QString error) {
            qDebug() << "Download failed: " << error;
       })
      .onSuccess([](QString result) { // 可省略
            qDebug() << "success: " << result;
       })
      .onFailed([](QString err) { // 可省略
            qDebug() << "failed: " << err;
       })
      .exec();
```

## 2.13 失败重试
### 接口:
&emsp;&emsp;设置失败请求后的重试次数，默认值为0。
```cpp
HttpRequest &retry(int count);
```

&emsp;&emsp;重试次数执行完成后的信号槽/回调。
```cpp
HttpRequest &onRetried(const QObject *receiver, const char *method);
HttpRequest &onRetried(std::function<void ()> lambda);
```

### 例子:
```cpp
client.get("xxx://httpbin.org/get")
      .retry(2) // 失败重试的次数
      .onRetried([](){qDebug()<<"retried!";}) // 失败重试操作完成后的回调
      .onSuccess([](QString result){qDebug()<<result;})
      .onFailed([](QString err){qDebug()<<err;})
      .exec();
```

## 2.14 重复请求
### 接口:
&emsp;&emsp;设置需要重复请求的次数，默认值为1。
```cpp
HttpRequest &repeat(int count);
```

&emsp;&emsp;重复请求完成后的信号槽/回调。
```cpp
HttpRequest &onRepeated(const QObject *receiver, const char *method);
HttpRequest &onRepeated(std::function<void ()> lambda);
```

### 例子:
```cpp
client.get("https://httpbin.org/get")
      .repeat(3) // 总共重复请求的次数
      .onRepeated([](){qDebug()<<"repeated!";}) // 重复请求操作完成后的回调
      .onSuccess([](QString result){qDebug()<<result;})
      .onFailed([](QString err){qDebug()<<err;})
      .exec();
```

## 2.15 身份验证
### 接口：
1. 自动填写身份验证信息。注：当QNetworkAccessManager的authenticationRequired信号触发时，会自动往QAuthenticator填写用户名和密码信息。  
```cpp
HttpRequest &autoAuthenticationRequired(const QAuthenticator &authenticator);
HttpRequest &autoAuthenticationRequired(const QString &user, const QString &password);
```

2. 通过回调/槽函数手动填写身份验证信息。
```cpp
inline HttpRequest &onAuthenticationRequired(const QObject *receiver, const char *method);
inline HttpRequest &onAuthenticationRequired(std::function<void (QAuthenticator *)> lambda);
```

3. 设置限制身份验证的次数，超过身份验证计数则触发失败并中断请求。  

变量 | 解释 
--- | ---
`count < 0` | 不限制验证次数
`count = 0` | 不验证 
`count = 1` | 限制验证1次(默认值)
`count > 0` | 限制count次

```cpp
inline HttpRequest &authenticationRequiredCount(int count = 1);
```

4. 身份验证失败后错误回调。
```cpp
inline HttpRequest &onAuthenticationRequireFailed(const QObject *receiver, const char *method);
inline HttpRequest &onAuthenticationRequireFailed(std::function<void ()> lambda);
inline HttpRequest &onAuthenticationRequireFailed(std::function<void (QNetworkReply *)> lambda);
```

### 例子
1. 身份验证
```cpp
client.get("https://httpbin.org/basic-auth/admin/123456")
      .onAuthenticationRequired([](QAuthenticator *authenticator) {
            authenticator->setUser("admin");
            authenticator->setPassword("123456");
            qDebug() << "=============";
        })
      .onSuccess([](QString result){qDebug()<<"success: "<<result;})
      .onFailed([](QString err){qDebug()<<"failed: "<<err;})
      .exec();
```

2. 身份自动验证
```cpp
client.get("https://httpbin.org/basic-auth/admin/123456")
      .autoAuthenticationRequired("admin", "123456")
      .onSuccess([](QString result){qDebug()<<"success: "<<result;})
      .onFailed([](QString err){qDebug()<<"failed: "<<err;})
      .exec();
```

3. 身份验证次数与错误处理
```cpp
client.get("https://httpbin.org/basic-auth/admin/123456")
      .authenticationRequiredCount(2) // 最大重试验证次数为2次(默认值为1次)
      .onAuthenticationRequired([](QAuthenticator *authenticator) {
            authenticator->setUser("admin");
            authenticator->setPassword("1234563"); // 错误输入
        })
      .onAuthenticationRequireFailed([](){ // 验证身份失败的回调
            qDebug() << "authentication failed!";
        })
      .onSuccess([](QString result){qDebug()<<"success: "<<result;})
      .onFailed([](QString err){qDebug()<<"failed: "<<err;})
      .exec();
```

## 注意
* `autoAuthenticationRequired`和`onAuthenticationRequired`不能同时使用，因为它们是互斥实现。如不慎同时使用，只会生效`autoAuthenticationRequired`操作。

# 3. 扫码关注微信公众号:Qt 君，第一时间获取推送。

<p align="center">
  <img src="http://www.qtbig.com/about/index/my_qrcode.jpg" alt="微信公众号:Qt君">
  <p align="center"><em>Qt君</em></p>
</p>

# 源码地址
https://github.com/aeagean/QtNetworkService
