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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Downloader d;
    d.setWindowTitle("断点续传下载器@Qt君");
    d.show();

    return a.exec();
}
