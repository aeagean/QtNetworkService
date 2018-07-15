CONFIG += c++11
CONFIG += staticlib
TEMPLATE = lib

unix:TARGET = $$PWD/lib/QtNetworkService
win32: {
DESTDIR = $$PWD/lib/
TARGET = QtNetworkService
}

SOURCES += main.cpp \
    Network/HttpService.cpp \
    Test.cpp \
    Network/HttpResponse.cpp \
    Network/HttpRequest.cpp \
    NetworkDownloadFile/NetworkDownloadFile.cpp

HEADERS += \
    Network/HttpService.h \
    Test.h \
    Network/HttpResponse.h \
    Network/HttpRequest.h \
    NetworkDownloadFile/NetworkDownloadFile.h

include(QtNetworkService.pri)
message(" ================ QtNetworkService Library ================ ")
