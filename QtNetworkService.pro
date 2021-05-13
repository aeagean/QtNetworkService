#**********************************************************
#Author: Qt君
#微信公众号: Qt君(文章首发)
#Website: qthub.com(后续更新)
#Email:  2088201923@qq.com
#QQ交流群: 732271126
#LISCENSE: MIT
#**********************************************************
QT += core
QT -= gui

CONFIG += c++11

DEFINES += QT_APP_DEBUG
DEFINES += QT_APP_MODE

contains(DEFINES, QT_APP_MODE) {
    HEADERS += Demo/Test1/ApiTest.h
    SOURCES += Demo/main.cpp \
               Demo/Test1/ApiTest.cpp
    INCLUDEPATH += Demo Demo/Test1 Demo/Test2

    message(" ================ QtNetworkService APPLICATION ================ ")
}
else {
    CONFIG += staticlib
    TEMPLATE = lib
    unix: TARGET = $$PWD/Lib/QtNetworkService

    win32: {
        DESTDIR = $$PWD/Lib/
        TARGET = QtNetworkService
    }

    message(" ================ QtNetworkService Library ================ ")
}

include(QtNetworkService.pri)
