#ifndef __DOWNLOAD_MANAGER__
#define __DOWNLOAD_MANAGER__

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include "common_stuff.h"

class MATHTOOL_EXPORT DownloadManager: public QObject
{
    Q_OBJECT
public:
    DownloadManager(QObject *parent = 0);

    void append(const QUrl &url);
    void append(const QStringList &urlList);
    void setOutputDir(const QString& dir);
    QString saveFileName(const QUrl &url);

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager manager;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    QTime downloadTime;

    int downloadedCount;
    int totalCount;
    QString _currentDir;
};

#endif //__DOWNLOAD_MANAGER__
