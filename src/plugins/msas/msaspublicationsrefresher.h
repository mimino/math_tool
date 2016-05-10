#ifndef MSASPUBLICATIONSREFRESHER_H
#define MSASPUBLICATIONSREFRESHER_H

#include <QList>
#include <QDomElement>

#include "msasrefresherbase.h"
#include "publication.h"

class DownloadManager;

class MSASPublicationsRefresher : public MSASRefresherBase
{
    Q_OBJECT
public:
    MSASPublicationsRefresher();
    virtual ~MSASPublicationsRefresher();
    virtual void refresh(int subDomain = 0);

private slots:
    void finished();

private:
	void downloadPage(int pageNum);
    void parseHtml(QString fileName);
    bool isLastPage(QString fileName);


	void switchState();
	void refreshList();
	void refreshPublications();
	void parseDigestList(QString fileName);
	Publication parsePublicationDigest(QDomElement& element);
	//void parseRelations(QString fileName);
	//void cleanRelationsList();

private:
	enum STATE
	{
		eIdle = 0,
		eDigests,
		ePublications
	} _currentState;

private:
    int _subDomain;
    DownloadManager* _downloadMgr;
    QList<Publication> _publications;
};

#endif // MSASPUBLICATIONSREFRESHER_H
