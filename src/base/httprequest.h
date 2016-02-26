#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "common_stuff.h"
#include <QNetworkAccessManager>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QFile;
class QSslError;
class QAuthenticator;
class QNetworkReply;
QT_END_NAMESPACE


class MATHTOOL_EXPORT HttpRequest : public QObject
{
    Q_OBJECT

public:
    HttpRequest();
    void downloadFile(QUrl url, QString filename);

signals:
    void downloadFinished();

private slots:
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);
#endif

private:
    void startRequest(QUrl url);
private:
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
};

#endif //__HTTP_REQUEST_H__
