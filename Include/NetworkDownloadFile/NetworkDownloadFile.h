/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/
#ifndef NETWORK_DOWNLOAD_FILE_H
#define NETWORK_DOWNLOAD_FILE_H

#include "HttpService.h"

#include <QFileInfo>

namespace AeaQt {

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

}
#endif // NETWORK_DOWNLOAD_FILE_H
