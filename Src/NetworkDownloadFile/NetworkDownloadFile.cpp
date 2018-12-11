/**********************************************************
Author: 微信公众号(你才小学生)
WeChat Official Accounts Platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
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
