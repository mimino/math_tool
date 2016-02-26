
#include <QDomDocument>
#include <QFile>

#include "downloadmanager.h"
#include "msaspublication.h"
#include "msaspublicationsrefresher.h"

MSASPublicationsRefresher::MSASPublicationsRefresher()
{
    _downloadMgr = new DownloadManager();
}

MSASPublicationsRefresher::~MSASPublicationsRefresher()
{
    _publications.clear();
    delete _downloadMgr;
}

void MSASPublicationsRefresher::refresh()
{
    QStringList list;
    for(int i=0; i<1000; i++)
    {
        QString url = QString("http://academic.research.microsoft.com/RankList?entitytype=1&topDomainID=15&subDomainID=7&last=0&start=%1&end=%2").arg(i*100+1).arg((i+1)*100);
        list.append(url);
    }

    _downloadMgr->append(list);
}

void MSASPublicationsRefresher::finished()
{
//    QString fileName(QString("page-%1").arg(nextPageNum));
//    parseHtml(fileName);
    bool lastPage = false;//isLastPage(fileName);
    if(!lastPage)
    {
        downloadPage(++nextPageNum);
    }
    else
    {
        //delete _httpRequest;
        emit(refreshFinished());
    }
}

void MSASPublicationsRefresher::parseHtml(QString fileName)
{
    QFile* file = new QFile(fileName);
    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString fff = file->readAll();
    fff.replace("&", "");

    QDomDocument xmldoc;
    xmldoc.setContent(fff);
    QDomElement docElem = xmldoc.documentElement();
    QDomNodeList list = docElem.elementsByTagName("tr");

    //Check each node one by one.
    for(int ii = 0;ii < list.count(); ii++)
    {
        QDomElement el = list.at(ii).toElement();

        MSASPublication article;
        article.parse(el);
        if(article.isInitialized())
        {
            _publications.append(article);
        }
    }

    file->close();
    delete file;
}
