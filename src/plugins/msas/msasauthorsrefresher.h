#ifndef __MSAS_AUTHORS_REFRESHER__
#define __MSAS_AUTHORS_REFRESHER__
#include <QList>
#include "msasrefresherbase.h"

class HttpRequest;
class MSASPublication;

class MSASAuthorsRefresher : public MSASRefresherBase
{
    Q_OBJECT
public:
    MSASAuthorsRefresher();
    virtual ~MSASAuthorsRefresher();
    virtual void refresh();

private slots:
    void finished();

private:
    HttpRequest* _httpRequest;
};

#endif // __MSAS_AUTHORS_REFRESHER__
