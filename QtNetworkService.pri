#**********************************************************
#Author: 微信公众号(你才小学生)
#WeChat public platform: nicaixiaoxuesheng
#Email:  2088201923@qq.com
#**********************************************************/
INCLUDEPATH += $$PWD/Include/Network \
               $$PWD/Include/NetworkDownloadFile \
               $$PWD/Include/Socket

QT += network

!CONFIG(QT_APP_MODE) {
    LIBS += -L$$PWD/Lib -lQtNetworkService
}
