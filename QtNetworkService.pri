INCLUDEPATH += $$PWD/Include/Network $$PWD/Include/NetworkDownloadFile

QT += network

!CONFIG(QT_APP_MODE) {
    LIBS += -L$$PWD/Lib -lQtNetworkService
}
