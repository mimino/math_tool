
#include <QWebView>
#include <QWebFrame>
#include <QWebElementCollection>

#include "msas_plugin.h"
#include "httprequest.h"
#include "msaspublicationsrefresher.h"
#include "msasauthorsrefresher.h"

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

int MSASPlugin::refresh()
{
    switchState();
    if(_refresher != NULL)
    {
        connect(_refresher, SIGNAL(refreshFinished()), SLOT(finished()));
        _refresher->refresh();
    }
    return 0;
}

void MSASPlugin::finished()
{
    if(_currentState != eIdle)
    {
        refresh();
    }
}

void MSASPlugin::switchState()
{
    switch(_currentState)
    {
    case eIdle:
        _currentState = ePublications;
        delete _refresher;
        _refresher = new MSASPublicationsRefresher();
        break;
    case ePublications:
        _currentState = eAuthors;
        delete _refresher;
        _refresher = NULL;
        _refresher = new MSASAuthorsRefresher();
        break;
    case eAuthors:
        _currentState = eIdle;
        delete _refresher;
        _refresher = NULL;
        break;
    default:
        break;
    };
}
