#**********************************************************
#* Author(作者)     : Qt君
#* 微信公众号        : Qt君
#* Website(网站)    : qthub.com
#* QQ交流群         : 1039852727
#* Email(邮箱)      : 2088201923@qq.com
#* Support(技术支持&合作) :2088201923(QQ)
#* Source Code(源码): https://github.com/aeagean/QtNetworkService
#* LISCENSE(开源协议): MIT
#*********************************************************/
QT += gui
QT += widgets

CONFIG += c++11

HEADERS += Downloader.h
SOURCES += main.cpp \
           Downloader.cpp

win32: RC_ICONS=app.ico

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

include($$PWD/../../src/QtNetworkService.pri)
