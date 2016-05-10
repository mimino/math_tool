#ifndef __KEYWORD__
#define __KEYWORD__

#include <QString>
#include <QVector3D>
#include "common_stuff.h"

class MATHTOOL_EXPORT Keyword
{
public:
    Keyword();
	Keyword(QString title, QString url, int weight);
    bool isInitialized() { return _title.length() != 0; }
public:
    QString _title;
    QString _url;
    int _weight;
};

#endif // __KEYWORD__
