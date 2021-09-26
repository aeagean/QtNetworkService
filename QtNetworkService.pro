#**********************************************************
#* Author(作者)     : Qt君
#* 微信公众号        : Qt君
#* Website(网站)    : qthub.com
#* QQ交流群         : 1039852727
#* Email(邮箱)      : 2088201923@qq.com
#* Support(技术支持&合作) :2088201923(QQ)
#* Source Code(源码): https://github.com/aeagean/QtNetworkService
#* LISCENSE(开源协议): MIT
#* Demo(演示):
#==========================================================
#  static AeaQt::HttpClient client;
#  client.get("https://qthub.com")
#        .onSuccess([](QString result) { qDebug()<<"success!"; })
#        .onFailed([](QString error) { qDebug()<<"failed!"; })
#        .exec();
#==========================================================
#*********************************************************/
QT += core
QT -= gui

CONFIG += c++11

#DEFINES += QT_APP_DEBUG
DEFINES += QT_APP_MODE

contains(DEFINES, QT_APP_MODE) {
    SOURCES += sample/main.cpp

    message(" ================ QtNetworkService Application ================ ")
}
else {
    CONFIG += staticlib
    TEMPLATE = lib
    unix: TARGET = $$PWD/lib/QtNetworkService

    win32: {
        DESTDIR = $$PWD/Lib/
        TARGET = QtNetworkService
    }

    message(" ================ QtNetworkService Library ================ ")
}

include($$PWD/src/QtNetworkService.pri)

DISTFILES += \
    README.md
