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
#ifndef QTHUB_COM_DOWNLOADER_H
#define QTHUB_COM_DOWNLOADER_H

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

public slots:
    void startDownload();
    void pauseDownload();
    void restoreDownload();

private slots:
    void onOpenFileDirClicked();
    void onStatisticsTimer();
    void onDownloadFileNameChanged(QString fileName);
    void onDownloadFileProgress(qint64 recv, qint64 total);
    void onDownloadFileSuccess(QString fileName);
    void onDownloadFileFailed(QString fileName);

private:
    bool isDownloading();

private:
    HttpClient    m_client;
    HttpResponse *m_response = nullptr;

    QLabel       *m_urlLabel              = new QLabel("下载链接");
    QLabel       *m_statusLabel           = new QLabel("已停止");
    QLabel       *m_speedLabel            = new QLabel("下载速度");
    QLabel       *m_speedValueLabel       = new QLabel("--");
    QLabel       *m_remainTimeLabel       = new QLabel("剩余时间");
    QLabel       *m_remainTimeValueLabel  = new QLabel("--");
    QLabel       *m_currentSizeLabel      = new QLabel("已下载");
    QLabel       *m_currentSizeValueLabel = new QLabel("--");
    QLabel       *m_fileSizeLabel         = new QLabel("总大小");
    QLabel       *m_fileSizeValueLabel    = new QLabel("--");
    QLabel       *m_fileNameLabel         = new QLabel("文件名字");
    QLabel       *m_fileNameValueLabel    = new QLabel("");
    QLineEdit    *m_urlEdit               = new QLineEdit("");
    QPushButton  *m_startBtn              = new QPushButton("开始");
    QPushButton  *m_pauseBtn              = new QPushButton("暂停");
    QPushButton  *m_restoreBtn            = new QPushButton("重新下载");
    QPushButton  *m_openFileDirBtn        = new QPushButton("打开目录");
    QProgressBar *m_progressBar           = new QProgressBar();

    QTimer       *m_statisticsTimer = new QTimer;
    QString       m_fileName        = "";
    qint64        m_lastRecvSize    = -1;
    qint64        m_currentRecvSize = -1;
    qint64        m_totalSize       = -1;
    bool          m_isDownloading   = false;
};

}
#endif // QTHUB_COM_DOWNLOADER_H
