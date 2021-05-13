#**********************************************************
#Author: Qt君
#微信公众号: Qt君(文章首发)
#Website: qthub.com(后续更新)
#Email:  2088201923@qq.com
#QQ交流群: 732271126
#LISCENSE: MIT
#**********************************************************
INCLUDEPATH += $$PWD/

SOURCES += \
    $$PWD/HttpResponse.cpp \
    $$PWD/HttpRequest.cpp \
    $$PWD/HttpClient.cpp

HEADERS += \
    $$PWD/HttpResponse.h \
    $$PWD/HttpRequest.h \
    $$PWD/HttpClient.h

QT += network
