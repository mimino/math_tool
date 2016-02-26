#ifndef MSASPUBLICATIONSREFRESHER_H
#define MSASPUBLICATIONSREFRESHER_H

#include <QList>
#include "msasrefresherbase.h"

class DownloadManager;
class MSASPublication;

class MSASPublicationsRefresher : public MSASRefresherBase
{
    Q_OBJECT
public:
    MSASPublicationsRefresher();
    virtual ~MSASPublicationsRefresher();
    virtual void refresh();

private slots:
    void finished();

private:

    void downloadPage(int pageNum);
    void parseHtml(QString fileName);
    bool isLastPage(QString fileName);

private:
    DownloadManager* _downloadMgr;
    QList<MSASPublication> _publications;
};

#endif // MSASPUBLICATIONSREFRESHER_H
