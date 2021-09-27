/**********************************************************
 * Author(作者)     : Qt君
 * 微信公众号        : Qt君
 * Website(网站)    : qthub.com
 * QQ交流群         : 1039852727
 * Email(邮箱)      : 2088201923@qq.com
 * Support(技术支持&合作) :2088201923(QQ)
 * Source Code(源码): https://github.com/aeagean/QtNetworkService
 * LISCENSE(开源协议): MIT
**********************************************************/
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "HttpClient.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

namespace AeaQt {

class Downloader : public QWidget
{
    Q_OBJECT
public:
    explicit Downloader(QWidget *parent = nullptr);

signals:

public slots:
    void startDownload();
    void pauseDownload();
    void restoreDownload();

private:
    bool isDownloading();

private:
    HttpClient m_client;
    QPushButton *m_startBtn = new QPushButton("开始");
    QPushButton *m_pauseBtn = new QPushButton("暂停");
    QPushButton *m_restoreBtn  = new QPushButton("重新下载");
    QLabel      *m_urlLabel = new QLabel("下载链接");
    QLabel      *m_statusLabel = new QLabel("已停止");
    QLabel      *m_speedLabel = new QLabel("下载速度");
    QLabel      *m_speedValueLabel = new QLabel("--");
    QLabel      *m_remainTimeLabel = new QLabel("剩余时间");
    QLabel      *m_remainTimeValueLabel = new QLabel("--");
    QLabel      *m_currentSizeLabel = new QLabel("已下载");
    QLabel      *m_currentSizeValueLabel = new QLabel("--");
    QLabel      *m_fileSizeLabel = new QLabel("总大小");
    QLabel      *m_fileSizeValueLabel = new QLabel("--");
    QLabel      *m_fileNameLabel = new QLabel("文件名字");
    QLabel      *m_fileNameValueLabel = new QLabel("");
    QPushButton *m_openFileDirBtn = new QPushButton("打开目录");
    QLabel      *m_infoLabel = new QLabel("");

    // https://github.com/aeagean/QtNetworkService/archive/refs/heads/master.zip
    // http://mirrors.tuna.tsinghua.edu.cn/qt/archive/qt/6.0/6.0.3/single/qt-everywhere-src-6.0.3.tar.xz
    QLineEdit   *m_urlEdit = new QLineEdit("https://github.com/aeagean/QtNetworkService/archive/refs/heads/master.zip");
    QProgressBar *m_progressBar = new QProgressBar();
    HttpResponse *m_response = nullptr;
    QString       m_fileName;
    QTimer       *m_statisticsTimer = new QTimer;
    qint64        m_lastRecvSize = -1;
    qint64        m_currentRecvSize = -1;
    qint64        m_totalSize = -1;
};

}
#endif // DOWNLOADER_H
