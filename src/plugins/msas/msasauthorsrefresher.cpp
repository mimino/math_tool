
#include <QDomDocument>
#include <QFile>

#include "author.h"
#include "downloadmanager.h"
#include "msasauthorsrefresher.h"


MSASAuthorsRefresher::MSASAuthorsRefresher()
{
    _downloadMgr = new DownloadManager();
}


MSASAuthorsRefresher::~MSASAuthorsRefresher()
{
    delete _downloadMgr;
}

void MSASAuthorsRefresher::refresh(int subDomain)
{
    QString dir("authors-subdomain-");
    dir.append(QString("%1").arg(subDomain));
    _downloadMgr->setOutputDir(dir);

    QStringList list;
    for(int i=0; i<2000; i++)
    {
        QString url = QString("http://academic.research.microsoft.com/RankList?entitytype=2&topDomainID=15&subDomainID=%1&last=0&start=%2&end=%3").arg(subDomain).arg(i*100+1).arg(100*(i+1));
        list.append(url);
    }

    connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
    _downloadMgr->append(list);
}

void MSASAuthorsRefresher::finished()
{
    emit(refreshFinished());
}
