#ifndef __MSAS_AUTHORS_REFRESHER__
#define __MSAS_AUTHORS_REFRESHER__


#include <QList>
#include <QMap>
#include <QDomElement>

#include "msasrefresherbase.h"

class Author;
class Keyword;
class DownloadManager;

class MSASAuthorsRefresher : public MSASRefresherBase
{
	Q_OBJECT
public:
	MSASAuthorsRefresher(QList<Author>& authors, QList<Keyword>& keywords);
	virtual ~MSASAuthorsRefresher();
	virtual void refresh(int subDomain = 0);

	private slots:
		void finished();

private:
	void switchState();
	void refreshList();
	void refreshAuthors();
	void parseDigestList(QString fileName);
	Author parseAuthorDigest(QDomElement& element);
	void parseRelations(QString fileName);
	void cleanRelationsList();
private:
	enum STATE
	{
		eIdle = 0,
		eDigests,
		eAuthors
	} _currentState;

private:
	int _subDomain;
	DownloadManager* _downloadMgr;
	QList<Keyword>& _keywords;
	QList<Author>& _authors;
	QMap<QString, int> _authorToIdMap;
	QMap<QString, int> _keywordToIdMap; 
};

#endif // __MSAS_AUTHORS_REFRESHER__
