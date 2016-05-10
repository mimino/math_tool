
#include <QDomDocument>
#include <QDir>
#include <QFile>

#include "downloadmanager.h"
#include "msaskeywordsrefresher.h"
#include "reg_exp.h"


MSASKeywordsRefresher::MSASKeywordsRefresher(QList<Keyword>& keywords, QList<QPair<int, int> >& relations) :
    /*_listRefresh(false),*/ _keywords(keywords), _relations(relations)
{
    _downloadMgr = new DownloadManager();
}


MSASKeywordsRefresher::~MSASKeywordsRefresher()
{
    delete _downloadMgr;
}

void MSASKeywordsRefresher::refresh(int subDomain)
{
    _subDomain = subDomain;
    refreshList();
}

void MSASKeywordsRefresher::switchState()
{
    switch(_currentState)
    {
    case eIdle:
        _currentState = eDigests;
        break;
    case eDigests:
        _currentState = eKeywords;
        break;
    case eKeywords:
        _currentState = eIdle;
        break;
    default:
        break;
    };
}

void MSASKeywordsRefresher::refreshList()
{
    _currentState = eDigests;

    QString dir("keywords-subdomain-");
    dir.append(QString("%1").arg(_subDomain));
    _downloadMgr->setOutputDir(dir);

    QStringList list;
    for(int i=0; i<200; i++)
    {
        QString url = QString("http://academic.research.microsoft.com/RankList?entitytype=8&topDomainID=15&subDomainID=%1&last=0&start=%2&end=%3").arg(_subDomain).arg(i*100+1).arg(100*(i+1));
        list.append(url);
    }

    connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
    _downloadMgr->append(list);
    //_listRefresh = true;
}

void MSASKeywordsRefresher::refreshKeywords()
{
    _currentState = eKeywords;

    QString dir = QString("keywords-subdomain-%1-full").arg(_subDomain);
    _downloadMgr->setOutputDir(dir);

    QStringList list;
    for(int i=0; i<_keywords.count(); i++)
    {
        QString url = QString("http://academic.research.microsoft.com%2").arg(_keywords.at(i)._url);
        list.append(url);
    }

    connect(_downloadMgr, SIGNAL(finished()), SLOT(finished()));
    _downloadMgr->append(list);
    //_listRefresh = false;
}

void MSASKeywordsRefresher::finished()
{
    switch(_currentState)
    {
    case eDigests:
    {
        QDir pluginsDir(QString("%1data/download/keywords-subdomain-%2").arg(APPLICATION_PATH).arg(_subDomain));
        foreach (QString fileName, pluginsDir.entryList(QDir::Files))
        {
            parseDigestList(QString("%1data/download/keywords-subdomain-%2/%3").arg(APPLICATION_PATH).arg(_subDomain).arg(fileName));
        }

        refreshKeywords();
        break;
    }
    case eKeywords:
    {
        int i=0;
        QDir pluginsDir(QString("%1data/download/keywords-subdomain-%2-full").arg(APPLICATION_PATH).arg(_subDomain));
        foreach (QString fileName, pluginsDir.entryList(QDir::Files))
        {
            QTime time(QTime::currentTime());
            qDebug() << "Iteration #" << i++ << "\n";
            parseRelations(QString("%1data/download/keywords-subdomain-%2-full/%3").arg(APPLICATION_PATH).arg(_subDomain).arg(fileName));
            QTime afterTime(QTime::currentTime());
            qDebug() << "Time: " << afterTime.msecsSinceStartOfDay() - time.msecsSinceStartOfDay() << "\n";
        }

        cleanRelationsList();

        _currentState = eIdle;
    }
    default:
        emit(refreshFinished());
        break;
    };
}

void MSASKeywordsRefresher::parseDigestList(QString fileName)
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

        Keyword keyword = parseKeywordDigest(el);
        if(keyword.isInitialized())
        {
            _keywords.append(keyword);
        }
    }

    file->close();
    delete file;
}

Keyword MSASKeywordsRefresher::parseKeywordDigest(QDomElement& element)
{
    Keyword res;
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

void MSASKeywordsRefresher::parseRelations(QString fileName)
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
    QString mainKeyword = list.at(0).toElement().text().trimmed();

    int index = -1;
    for(int i=0; i<_keywords.count(); i++)
    {
        if(_keywords.at(i)._title.compare(mainKeyword) == 0)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
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
            }

            for(int i=0; i<_keywords.count(); i++)
            {
                if(_keywords.at(i)._title.compare(keyword) == 0)
                {
                    relation.second = i;
                    if(relation.first != relation.second)
                    {
                        int change;
                        if(relation.first > relation.second)
                        {
                            change = relation.first;
                            relation.first = relation.second;
                            relation.second = change;
                        }

                        _relations.append(relation);
                    }
                    break;
                }
            }

            pos += rx.matchedLength();
        }
    }

    file->close();
    delete file;
}

void MSASKeywordsRefresher::cleanRelationsList()
{
    std::sort(_relations.begin(), _relations.end(), qLess<QPair<int, int> >());
    int count = _relations.count();
    for(int k=count-1; k>0; k--)
    {
        if(_relations[k] == _relations[k-1])
        {
            _relations.removeAt(k);
        }
    }
}
