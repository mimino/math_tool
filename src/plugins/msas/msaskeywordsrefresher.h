#ifndef __MSAS_KEYWORDS_REFRESHER__
#define __MSAS_KEYWORDS_REFRESHER__

#include <QList>
#include <QDomElement>

#include "msasrefresherbase.h"
#include "keyword.h"


class MSASAuthor;
class DownloadManager;

class MSASKeywordsRefresher : public MSASRefresherBase
{
    Q_OBJECT
public:
    MSASKeywordsRefresher(QList<Keyword>& keywords, QList<QPair<int, int> >& relations);
    virtual ~MSASKeywordsRefresher();
    virtual void refresh(int subDomain = 0);

private slots:
    void finished();

private:
    void switchState();
    void refreshList();
    void refreshKeywords();
    void parseDigestList(QString fileName);
    Keyword parseKeywordDigest(QDomElement& element);
    void parseRelations(QString fileName);
    void cleanRelationsList();

private:
    enum STATE
    {
        eIdle = 0,
        eDigests,
        eKeywords
    } _currentState;

    int _subDomain;
    DownloadManager* _downloadMgr;
    bool _listRefresh;
    QList<Keyword>& _keywords;
    QList<QPair<int, int> >& _relations;
};

#endif // __MSAS_KEYWORDS_REFRESHER__
