#ifndef __MSAS_AUTHORS_REFRESHER__
#define __MSAS_AUTHORS_REFRESHER__


#include <QList>
#include "msasrefresherbase.h"

class Author;
class DownloadManager;

class MSASAuthorsRefresher : public MSASRefresherBase
{
    Q_OBJECT
public:
    MSASAuthorsRefresher();
    virtual ~MSASAuthorsRefresher();
    virtual void refresh(int subDomain = 0);

private slots:
    void finished();

private:
    DownloadManager* _downloadMgr;
    QList<Author> _authors;
};

#endif // __MSAS_AUTHORS_REFRESHER__
