#include "NetworkDownloadFile.h"

NetworkDownloadFile::NetworkDownloadFile()
{
    download("", QFileInfo());
}

void NetworkDownloadFile::download(const QString &url, const QFileInfo &fileInfo,
                                   std::function<void (QString)> successFunctor,
                                   std::function<void (QString)> errorFunctor,
                                   std::function<void (qint64, qint64)> processFunctor)
{

}
