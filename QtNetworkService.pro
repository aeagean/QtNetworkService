#**********************************************************
#Author: 微信公众号(你才小学生)
#WeChat public platform: nicaixiaoxuesheng
#Email:  2088201923@qq.com
#**********************************************************/
CONFIG += c++11

#CONFIG += QT_APP_MODE

CONFIG(QT_APP_MODE) {
    QT      += core
    HEADERS += Demo/Test1/ApiTest.h
    SOURCES += Demo/main.cpp \
               Demo/Test1/ApiTest.cpp
    INCLUDEPATH += Demo Demo/Test1 Demo/Test2
    message(" ================ QtNetworkService APPLICATION ================ ")
}
else {
    CONFIG += staticlib
    TEMPLATE = lib
    unix:TARGET = $$PWD/Lib/QtNetworkService

    win32: {
    DESTDIR = $$PWD/Lib/
    TARGET = QtNetworkService
    }

    message(" ================ QtNetworkService Library ================ ")
}

SOURCES += \
    Src/Network/HttpService.cpp \
    Src/Network/HttpResponse.cpp \
    Src/Network/HttpRequest.cpp \
    Src/NetworkDownloadFile/NetworkDownloadFile.cpp \
    Src/Socket/UdpServer.cpp \
    Src/Socket/UdpClient.cpp

HEADERS += \
    Include/Network/HttpService.h \
    Include/Network/HttpResponse.h \
    Include/Network/HttpRequest.h \
    Include/NetworkDownloadFile/NetworkDownloadFile.h \
    Include/Socket/UdpServer.h \
    Include/Socket/UdpClient.h

include(QtNetworkService.pri)
