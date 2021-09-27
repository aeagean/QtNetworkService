/**********************************************************
 * Author(作者)     : Qt君
 * 微信公众号        : Qt君
 * Website(网站)    : qthub.com
 * QQ交流群         : 1039852727
 * Email(邮箱)      : 2088201923@qq.com
 * Support(技术支持&合作) :2088201923(QQ)
 * Source Code(源码): https://github.com/aeagean/QtNetworkService
 * LISCENSE(开源协议): MIT
**********************************************************/
#include "Downloader.h"
#include <QApplication>
#include <QDebug>

using namespace AeaQt;

// Qt 5.12.1 msvc question head
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 0
    HttpClient c;
//    c.head("https://github.com/aeagean/QtNetworkService/archive/refs/heads/master.zip")
    c.get("http://mirrors.tuna.tsinghua.edu.cn/qt/archive/qt/6.0/6.0.3/single/qt-everywhere-src-6.0.3.tar.xz")
    .download()
    .onHead([](QMap<QString, QString> map) { qDebug() << ">" << map;})
    .onDownloadFileNameChanged([](QString fileName){ qDebug() << "file: " << fileName;})
    .onFinished([](QByteArray s){ qDebug() << "success:" << s.size();})
    .onFailed([](QByteArray s){ qDebug() << "failed:" << s;})
    .exec();
#else
    Downloader d;
    d.setWindowTitle("下载器@Qt君");
    d.show();
#endif

    return a.exec();
}
