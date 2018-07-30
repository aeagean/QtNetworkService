INCLUDEPATH += $$PWD/Include/Network \
               $$PWD/Include/NetworkDownloadFile \
               $$PWD/Include/Socket

QT += network

LIBS += -L$$PWD/Lib -lQtNetworkService
