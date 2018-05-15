#ifndef NETWORKDOWNLOADFILE_H
#define NETWORKDOWNLOADFILE_H

#include "HttpService.h"

#include <QFileInfo>

class NetworkDownloadFile : public QObject
{
    Q_OBJECT
public:
    NetworkDownloadFile();

    void download(const QString &url, const QFileInfo &fileInfo,
                  std::function<void(QString fileSavePath)> successFunctor = {},
                  std::function<void(QString errorString)> errorFunctor = {},
                  std::function<void(qint64 bytesReceived, qint64 bytesTotal)> processFunctor = {});
private:
    HttpService m_httpService;
};

#endif // NETWORKDOWNLOADFILE_H
