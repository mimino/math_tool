
#include <QDomDocument>
#include <QFile>
#include <QDir>

#include "downloadmanager.h"
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

void MSASPublicationsRefresher::refresh(int subDomain)
{
	_subDomain = subDomain;
	refreshList();
}

void MSASPublicationsRefresher::refreshList()
{
	_currentState = eDigests;

	QString dir("publications-subdomain-");
	dir.append(QString("%1").arg(_subDomain));
	_downloadMgr->setOutputDir(dir);

	QStringList list;
	for(int i=0; i<2000; i++)
	{
		QString url = QString("http://academic.research.microsoft.com/RankList?entitytype=1&topDomainID=15&subDomainID=%1&last=0&start=%2&end=%3").arg(_subDomain).arg(i*100+1).arg((i+1)*100);
		list.append(url);
	}

	connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
	_downloadMgr->append(list);
}

void MSASPublicationsRefresher::finished()
{
	switch(_currentState)
	{
	case eDigests:
		{
			QDir pluginsDir(QString("download/publications-subdomain-%1").arg(_subDomain));
			foreach (QString fileName, pluginsDir.entryList(QDir::Files))
			{
				parseDigestList(QString("download/publications-subdomain-%1/%2").arg(_subDomain).arg(fileName));
			}

			refreshPublications();
			break;
		}
	case ePublications:
		{
			_currentState = eIdle;
		}
	default:
		emit(refreshFinished());
		break;
	};
}

void MSASPublicationsRefresher::refreshPublications()
{
	_currentState = ePublications;

	QString dir = QString("publications-subdomain-%1-full").arg(_subDomain);
	_downloadMgr->setOutputDir(dir);

	QStringList list;
	for(int i=0; i<_publications.count(); i++)
	{
		QString url = QString("http://academic.research.microsoft.com%2").arg(_publications[i]._url);
		list.append(url);
	}

	connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
	_downloadMgr->append(list);
	//_listRefresh = false;
}

void MSASPublicationsRefresher::parseDigestList(QString fileName)
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

		Publication publication = parsePublicationDigest(el);
		if(publication.isInitialized())
		{
			_publications.append(publication);
		}
	}

	file->close();
	delete file;
}

Publication MSASPublicationsRefresher::parsePublicationDigest(QDomElement& element)
{
	Publication res;
	QDomNodeList aList = element.elementsByTagName("a");
	int count = aList.count();
	for(int i=0; i<count; i++)
	{
		QDomNode node = aList.at(i);
		QDomElement subelem = node.toElement();

		if(subelem.hasAttribute("id") && subelem.attribute("id").contains("ctl00_MainContent_ct"))
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
