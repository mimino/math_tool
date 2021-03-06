#include "downloadmanager.h"

#include <QtNetwork>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <stdio.h>


DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent), downloadedCount(0), totalCount(0)
{
}

void DownloadManager::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
    {
        append(QUrl::fromEncoded(url.toLocal8Bit()));
    }

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append(const QUrl &url)
{
    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    downloadQueue.enqueue(url);
    ++totalCount;
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

void DownloadManager::startNextDownload()
{
    QNetworkProxyFactory::setUseSystemConfiguration( true );
    QList<QNetworkProxy> proxyList = QNetworkProxyFactory::systemProxyForQuery(QNetworkProxyQuery(QUrl("http://www.yandex.ru")));
    QString proxyAddress;

    if( !proxyList.empty() )
    {
        if( !proxyList[0].hostName().isEmpty() )
        {
            proxyAddress = proxyList[0].hostName() + ":" + QString::number(proxyList[0].port());

            QNetworkProxy nwProxy;
            nwProxy.setType(QNetworkProxy::HttpProxy);
            nwProxy.setHostName(proxyList[0].hostName());
            nwProxy.setPort(proxyList[0].port());

            manager.setProxy(nwProxy);
        }
    }

    if (downloadQueue.isEmpty()) {
        qDebug() << downloadedCount << "/" << totalCount << " files downloaded successfully\n";
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();

    QString filename = saveFileName(url);
    output.setFileName(filename);
    if (!output.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
                qPrintable(filename), url.toEncoded().constData(),
                qPrintable(output.errorString()));

        startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    currentDownload = manager.get(request);
    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    // prepare the output
    qDebug() << "Downloading " << url.toEncoded().constData();
    downloadTime.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
}

void DownloadManager::downloadFinished()
{
    output.flush();
    output.close();

    if (currentDownload->error()) {
        // download failed
        qDebug() << "Failed: " << qPrintable(currentDownload->errorString());
    } else {
        qDebug() << "Succeeded.\n";
        ++downloadedCount;
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    QByteArray array = currentDownload->readAll();
    output.write(array);
}

