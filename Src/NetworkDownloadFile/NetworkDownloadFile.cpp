/**********************************************************
Author: Qt君
微信公众号: Qt君(文章首发)
Website: qtbig.com(后续更新)
Email:  2088201923@qq.com
QQ交流群: 732271126
LISCENSE: MIT
**********************************************************/
#include "NetworkDownloadFile.h"

using namespace AeaQt;

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
