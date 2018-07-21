CONFIG += c++11
CONFIG += staticlib
TEMPLATE = lib

unix:TARGET = $$PWD/Lib/QtNetworkService
win32: {
DESTDIR = $$PWD/Lib/
TARGET = QtNetworkService
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
message(" ================ QtNetworkService Library ================ ")
