
#include <QDomDocument>
#include <QFile>

#include "httprequest.h"
#include "msaspublication.h"
#include "msasauthorsrefresher.h"


MSASAuthorsRefresher::MSASAuthorsRefresher()
{
}


MSASAuthorsRefresher::~MSASAuthorsRefresher()
{
}

void MSASAuthorsRefresher::refresh()
{
    finished();
}

void MSASAuthorsRefresher::finished()
{
    emit(refreshFinished());
}
