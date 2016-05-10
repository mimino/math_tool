
#include <QWebView>
#include <QWebFrame>
#include <QWebElementCollection>

#include "msas_plugin.h"
#include "msaspublicationsrefresher.h"
#include "msasauthorsrefresher.h"
#include "msaskeywordsrefresher.h"

MSASPlugin::MSASPlugin() : _refresher(NULL)
{
    _currentState = eIdle;
}

MSASPlugin::~MSASPlugin()
{
}

QString MSASPlugin::name()
{
    QString name("Microsoft Academic Search");
    return name;
}

QString MSASPlugin::dataDir()
{
    QString res("msas");
    return res;
}

int MSASPlugin::refresh(int subjectId)
{
    _currentSubjectId = subjectId;

    switchState();
    if(_refresher != NULL)
    {
        connect(_refresher, SIGNAL(refreshFinished()), SLOT(finished()));
        _refresher->refresh(subjectIdToSubdomain(subjectId));
    }
    else
    {
        finished();
    }

    return 0;
}

void MSASPlugin::getKeywords(int subjectId, QList<Keyword>& keywords, QList<QPair<int, int> >& relations)
{
    keywords = _keywords;
    relations = _relations;
}

void MSASPlugin::finished()
{
    if(_currentState != eIdle)
    {
        refresh(_currentSubjectId);
    }
    else
    {
        emit(pluginRefreshFinished(this));
    }
}

void MSASPlugin::switchState()
{
    switch(_currentState)
    {
    case eIdle:
		_currentState = eKeywords;
		delete _refresher;
		_refresher = new MSASKeywordsRefresher(_keywords, _relations);
		break;
	case ePublications:
		_currentState = eIdle;
		delete _refresher;
		_refresher = NULL;
		break;
    case eAuthors:
		//_currentState = ePublications;
		//delete _refresher;
		//_refresher = new MSASPublicationsRefresher();
       
		_currentState = eIdle;
		delete _refresher;
		_refresher = NULL;
		break;
    case eKeywords:
		_currentState = eAuthors;
		delete _refresher;
		_refresher = new MSASAuthorsRefresher();
        break;
    default:
        break;
    };
}

int MSASPlugin::subjectIdToSubdomain(int subjectId)
{
    int subdomains [13] = {0, 1, 3, 4, 7, 8, 2, 10, 5, 11, 12, 9, 6};
    return subdomains[subjectId];
}
