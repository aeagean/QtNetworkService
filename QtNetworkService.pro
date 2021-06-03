#**********************************************************
#Author: Qt君
#微信公众号: Qt君
#Website: qthub.com
#Email:  2088201923@qq.com
#QQ交流群: 732271126
#Source Code: https://github.com/aeagean/QtNetworkService
#LISCENSE: MIT
#**********************************************************
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
