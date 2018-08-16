CONFIG += c++11

CONFIG(QT_APP_MODE) {
    QT      += core
    HEADERS += Demo/Test1/ApiTest.h
    SOURCES += Demo/main.cpp \
               Demo/Test1/ApiTest.cpp
    INCLUDEPATH += Demo Demo/Test1 Demo/Test2
    message(" ================ QtNetworkService APPLICATION ================ ")
}
else {
    CONFIG += staticli
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
    Src/NetworkDownloadFile/NetworkDownloadFile.cpp

HEADERS += \
    Include/Network/HttpService.h \
    Include/Network/HttpResponse.h \
    Include/Network/HttpRequest.h \
    Include/NetworkDownloadFile/NetworkDownloadFile.h

include(QtNetworkService.pri)
