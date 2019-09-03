/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#ifndef NETWORK_DOWNLOAD_FILE_H
#define NETWORK_DOWNLOAD_FILE_H

#include "HttpClient.h"

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
    HttpClient m_httpService;
};

}
#endif // NETWORK_DOWNLOAD_FILE_H
