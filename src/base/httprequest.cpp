#include <QtGui>
#include <QtNetwork>
#include <QtWebKit/QtWebKit>

#include "httprequest.h"

const QString UrlBase = "http://arxiv.org/list/math/";

HttpRequest::HttpRequest()
{
}

void HttpRequest::downloadFile(QUrl url, QString filename)
{
    if (QFile::exists(filename))
    {
        QFile::remove(filename);
    }

    file = new QFile(filename);
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);
}

void HttpRequest::startRequest(QUrl url)
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

            qnam.setProxy(nwProxy);
        }
    }

    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void HttpRequest::cancelDownload()
{
    httpRequestAborted = true;
    reply->abort();
}

void HttpRequest::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        return;
    }

    file->flush();
    file->close();


    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
    } else if (!redirectionTarget.isNull()) {        
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
    } 

    qDebug() << "finished, error = " << reply->error();
    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;

    emit(downloadFinished());
}

void HttpRequest::httpReadyRead()
{
    if (file)
    {
        QByteArray array = reply->readAll();
        file->write(array);
    }
}

void HttpRequest::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;
}

void HttpRequest::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
//    QDialog dlg;
//    Ui::Dialog ui;
//    ui.setupUi(&dlg);
//    dlg.adjustSize();
//    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(url.host()));

//    // Did the URL have information? Fill the UI
//    // This is only relevant if the URL-supplied credentials were wrong
//    ui.userEdit->setText(url.userName());
//    ui.passwordEdit->setText(url.password());

//    if (dlg.exec() == QDialog::Accepted) {
//        authenticator->setUser(ui.userEdit->text());
//        authenticator->setPassword(ui.passwordEdit->text());
//    }
}

#ifndef QT_NO_OPENSSL
void HttpRequest::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }
    
//    if (QMessageBox::warning(this, tr("HTTP"),
//                             tr("One or more SSL errors has occurred: %1").arg(errorString),
//                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
//        reply->ignoreSslErrors();
//    }
}
#endif
