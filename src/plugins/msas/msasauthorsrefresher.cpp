
#include <QDomDocument>
#include <QDir>
#include <QFile>

#include "author.h"
#include "keyword.h"
#include "downloadmanager.h"
#include "msasauthorsrefresher.h"
#include "reg_exp.h"


MSASAuthorsRefresher::MSASAuthorsRefresher(QList<Author>& authors, QList<Keyword>& keywords) : 
_authors(authors), _keywords(keywords)
{
    _downloadMgr = new DownloadManager();
}


MSASAuthorsRefresher::~MSASAuthorsRefresher()
{
    delete _downloadMgr;
}

void MSASAuthorsRefresher::refresh(int subDomain)
{
	_subDomain = subDomain;
	refreshList();
}

void MSASAuthorsRefresher::refreshList()
{
	_currentState = eDigests;

	QString dir("authors-subdomain-");
	dir.append(QString("%1").arg(_subDomain));
	_downloadMgr->setOutputDir(dir);

	QStringList list;
	for(int i=0; i<2000; i++)
	{
		QString url = QString("http://academic.research.microsoft.com/RankList?entitytype=2&topDomainID=15&subDomainID=%1&last=0&start=%2&end=%3").arg(_subDomain).arg(i*100+1).arg(100*(i+1));
		list.append(url);
	}

	connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
	_downloadMgr->append(list);
}

void MSASAuthorsRefresher::finished()
{
	switch(_currentState)
	{
	case eDigests:
		{
			QDir pluginsDir(QString("%1data/download/authors-subdomain-%2").arg(APPLICATION_PATH).arg(_subDomain));
			foreach (QString fileName, pluginsDir.entryList(QDir::Files))
			{
				parseDigestList(QString("%1data/download/authors-subdomain-%2/%3").arg(APPLICATION_PATH).arg(_subDomain).arg(fileName));
			}

			refreshAuthors();
			break;
		}
	case eAuthors:
		{
			for(int k=0; k<_authors.count(); k++)
			{
				_authorToIdMap.insert(_authors[k]._title, k);
			}

			for(int k=0; k<_keywords.count(); k++)
			{
				_keywordToIdMap.insert(_keywords[k]._title, k);
			}

			int i=0;
			QDir pluginsDir(QString("%1data/download/authors-subdomain-%2-full").arg(APPLICATION_PATH).arg(_subDomain));
			foreach (QString fileName, pluginsDir.entryList(QDir::Files))
			{
				QTime time(QTime::currentTime());
				qDebug() << "Authors: Iteration #" << i++ << "\n";
				parseRelations(QString("%1data/download/authors-subdomain-%2-full/%3").arg(APPLICATION_PATH).arg(_subDomain).arg(fileName));
				QTime afterTime(QTime::currentTime());
				qDebug() << "Authors time: " << afterTime.msecsSinceStartOfDay() - time.msecsSinceStartOfDay() << "\n";
			}

			cleanRelationsList();

			_authorToIdMap.clear();
			_keywordToIdMap.clear();

			_currentState = eIdle;
		}
	default:
		emit(refreshFinished());
		break;
	};
}

void MSASAuthorsRefresher::refreshAuthors()
{
	_currentState = eAuthors;

	QString dir = QString("authors-subdomain-%1-full").arg(_subDomain);
	_downloadMgr->setOutputDir(dir);

	QStringList list;
	for(int i=0; i<_authors.count(); i++)
	{
		QString url = _authors[i]._url;
		list.append(url);
	}

	connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
	_downloadMgr->append(list);
}

void MSASAuthorsRefresher::parseDigestList(QString fileName)
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

	int count = list.count();
	//Check each node one by one.
	for(int ii = 0;ii < list.count(); ii++)
	{
		QDomElement el = list.at(ii).toElement();

		Author author = parseAuthorDigest(el);
		if(author.isInitialized())
		{
			_authors.append(author);
		}
	}

	file->close();
	delete file;
}

Author MSASAuthorsRefresher::parseAuthorDigest(QDomElement& element)
{
	Author res;
	QDomNodeList aList = element.elementsByTagName("a");
	int count = aList.count();
	for(int i=0; i<count; i++)
	{
		QDomNode node = aList.at(i);
		QDomElement subelem = node.toElement();

		if(subelem.hasAttribute("id") && subelem.attribute("id").contains("AuthorSearchItem_authorName"))
		{
			res._url = subelem.attribute("href");
			res._title = subelem.text();
		}
	}

	aList = element.elementsByTagName("td");
	count = aList.count();
	for(int i=0; i<count; i++)
	{
		QDomNode node = aList.at(i);
		QDomElement subelem = node.toElement();

		if(!subelem.hasAttribute("id") && subelem.hasAttribute("class") && subelem.attribute("class").contains("staticOrderCol"))
		{
			QString sss = subelem.text();
			int val = sss.toInt();
			res._weight = subelem.text().toInt();
		}
	}

	return res;
}

void MSASAuthorsRefresher::parseRelations(QString fileName)
{
	QFile* file = new QFile(fileName);
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QString fff = file->readAll();
	fff.replace("&", "");

	QDomDocument xmldoc;
	xmldoc.setContent(fff);
	QDomElement docElem = xmldoc.documentElement();
	QDomNodeList list = docElem.elementsByTagName("title");
	QString author = list.at(0).toElement().text().trimmed();

	int index = -1;
	for(int i=0; i<_authors.count(); i++)
	{
		if(_authors.at(i)._title.compare(author) == 0)
		{
			index = i;
			break;
		}
	}

	if(index >= 0)
	{
		QString aaa;
		QDomNodeList coauthors = docElem.elementsByTagName("a");
		for(int k=0; k<coauthors.count(); k++)
		{
			QDomNode node = coauthors.at(k);
			QDomElement subelem = node.toElement();
			QString name = subelem.parentNode().nodeName();
			if(subelem.parentNode().nodeName().compare("li") == 0 
				&& subelem.attributes().contains("href")
				&& subelem.attribute("href").contains("/Author/"))
			{
				aaa = subelem.text();
				int pos = _authorToIdMap[aaa];
				if(!_authors.at(index)._coauthors.contains(pos))
				{
					_authors[index]._coauthors.append(pos);
				}
			}
		}

		QString res = RegExp::getByPattern("_keyword_data = \\[(.*)\\]", fff);

		QRegExp rx("\"(.*)__");
		rx.setCaseSensitivity(Qt::CaseSensitive);
		rx.setMinimal(true);
		rx.setPatternSyntax(QRegExp::RegExp);

		int pos = 0;
		while ((pos = rx.indexIn(res, pos)) != -1) {
			QPair<int, int> relation;
			relation.first = index;

			QString keyword = rx.cap(1);
			if(keyword.contains(","))
			{
				keyword = keyword.mid(3);
				int pos = _keywordToIdMap[keyword];
				if(!_authors.at(index)._keywords.contains(pos))
				{
					_authors[index]._keywords.append(pos);
				}
			}

			pos += rx.matchedLength();
		}


		/*QDomNodeList citations = docElem.elementsByTagName("a");
		for(int i=0; i<citations.count(); i++)
		{
			QDomElement citElem = citations.at(i).toElement();
			if(citElem.attributes().contains("id") && citElem.attribute("id").contains("MainContent_AuthorItem_citedBy"))
			{
				_authors[index]._weight = citElem.firstChild().toElement().text().toInt();
			}
		}*/
	}

	file->close();
	delete file;
}

void MSASAuthorsRefresher::cleanRelationsList()
{
	/*std::sort(_relations.begin(), _relations.end(), qLess<QPair<int, int> >());
	int count = _relations.count();
	for(int k=count-1; k>0; k--)
	{
		if(_relations[k] == _relations[k-1])
		{
			_relations.removeAt(k);
		}
	}*/
}
