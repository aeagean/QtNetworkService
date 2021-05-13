#**********************************************************
#Author: Qt君
#微信公众号: Qt君(文章首发)
#Website: qthub.com(后续更新)
#Email:  2088201923@qq.com
#QQ交流群: 732271126
#LISCENSE: MIT
#**********************************************************
INCLUDEPATH += $$PWD/Include/Network \

SOURCES += \
    Src/Network/HttpResponse.cpp \
    Src/Network/HttpRequest.cpp \
    Src/Network/HttpClient.cpp

HEADERS += \
    Include/Network/HttpResponse.h \
    Include/Network/HttpRequest.h \
    Include/Network/HttpClient.h

QT += network
