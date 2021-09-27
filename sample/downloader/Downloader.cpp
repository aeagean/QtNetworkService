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
#include "Downloader.h"
#include <QDir>
#include <QDesktopServices>

using namespace AeaQt;

#define _Byte (1)
#define KB (1024 * _Byte)
#define MB (1024 * KB)
#define GB (1024 * MB)

QString toBytes(qint64 v)
{
    if (v < 1 * MB) {
        return QString::number(v) + "KB";
    }
    else if (v >= MB && v < GB) {
        return QString::number(qreal(v)/MB, 'f', 2) + "MB";
    }
    else {
        return QString::number(qreal(v)/GB, 'f', 2) + "GB";
    }
}

Downloader::Downloader(QWidget *parent) : QWidget(parent)
{ 
    m_progressBar->setValue(0);

    QGridLayout *g = new QGridLayout;
    g->addWidget(m_urlLabel, 0, 0, 1, 1);
    g->addWidget(m_urlEdit,  0, 1, 1, 3);

    g->addWidget(m_fileNameLabel,      1, 0, 1, 1);
    g->addWidget(m_fileNameValueLabel, 1, 1, 1, 2);
    g->addWidget(m_openFileDirBtn,     1, 3, 1, 1);

    g->addWidget(m_progressBar, 2, 0, 1, 4);

    g->addWidget(m_speedLabel,           3, 0, 1, 1);
    g->addWidget(m_speedValueLabel,      3, 1, 1, 1);
    g->addWidget(m_remainTimeLabel,      3, 2, 1, 1);
    g->addWidget(m_remainTimeValueLabel, 3, 3, 1, 1);

    g->addWidget(m_currentSizeLabel,      4, 0, 1, 1);
    g->addWidget(m_currentSizeValueLabel, 4, 1, 1, 1);
    g->addWidget(m_fileSizeLabel,         4, 2, 1, 1);
    g->addWidget(m_fileSizeValueLabel,    4, 3, 1, 1);

    g->addWidget(m_statusLabel, 5, 0, 1, 1);
    g->addWidget(m_startBtn,    5, 1, 1, 1);
    g->addWidget(m_pauseBtn,    5, 2, 1, 1);
    g->addWidget(m_restoreBtn,  5, 3, 1, 1);

    this->setLayout(g);

    connect(m_startBtn,   &QPushButton::clicked, this, &Downloader::startDownload);
    connect(m_pauseBtn,   &QPushButton::clicked, this, &Downloader::pauseDownload);
    connect(m_restoreBtn, &QPushButton::clicked, this, &Downloader::restoreDownload);
    connect(m_openFileDirBtn, &QPushButton::clicked, [this](){
        QFileInfo fileInfo(m_fileName);
        bool ok = QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.dir().path()));
        qDebug() << "open url: " << ok;
    });

    m_statisticsTimer->start(1000);
    connect(m_statisticsTimer, &QTimer::timeout, [this](){
        if (m_lastRecvSize == -1) {
            m_lastRecvSize = m_currentRecvSize;
            return;
        }

        qint64 speed = (m_currentRecvSize - m_lastRecvSize) * 1000 / m_statisticsTimer->interval();
        qint64 remainSize = m_totalSize - m_currentRecvSize;

        if (speed >= 0) {
            m_speedValueLabel->setText(toBytes(speed));
        }

        m_lastRecvSize = m_currentRecvSize;

        if (speed > 0 && remainSize > 0) {
            m_remainTimeValueLabel->setText(QString::number(remainSize/speed) + "秒");
        }
        else {
            m_remainTimeValueLabel->setText(QString::number(0) + "秒");
        }
    });
}

void Downloader::startDownload()
{
    if (isDownloading())
        return;

    m_statusLabel->setText("正在下载...");
    m_statisticsTimer->start();

    m_response = m_client.get(m_urlEdit->text())
                .download()
                .enabledBreakpointDownload()
                .onDownloadFileNameChanged([this](QString fileName){
                    this->m_fileName = fileName;
                    m_fileNameValueLabel->setText(fileName);
                 })
                .onDownloadFileProgress([this](qint64 recv, qint64 total) {
                    m_currentRecvSize = recv;
                    m_totalSize = total;
                    m_currentSizeValueLabel->setText(toBytes(recv));
                    m_fileSizeValueLabel->setText(toBytes(total));
                    m_progressBar->setValue(100 * qreal(recv)/total);
                 })
                .onDownloadFileSuccess([this](QString fileName){
                    m_statusLabel->setText("下载完成");
                    qDebug() << "Download success: " << fileName;
                 })
                .onDownloadFileFailed([this](QString fileName){
                    m_statusLabel->setText("下载失败");
                    qDebug() << "Download failed: " << fileName;
                 })
                .exec();

    connect(m_response, &HttpResponse::destroyed, [this](){ m_response = nullptr; });
}

void Downloader::pauseDownload()
{
    if (isDownloading()) {
        m_response->reply()->abort();
    }

    m_statisticsTimer->stop();

    m_statusLabel->setText("已暂停");
}

void Downloader::restoreDownload()
{
    QFile::remove(m_fileName);
    this->startDownload();
}

bool Downloader::isDownloading()
{
    return m_response != nullptr;
}
