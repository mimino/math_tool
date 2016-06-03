#ifndef __AUTHOR__
#define __AUTHOR__

#include <QString>
#include <QList>
#include <QVector>
#include "common_stuff.h"

class MATHTOOL_EXPORT Author
{
public:
    Author();
	Author(QString title, QString url, int weight);
	bool isInitialized() { return _title.length() != 0; }
public:
	QString _title;
	QString _url;
	int _weight;
	QList<int> _coauthors;
	QList<int> _keywords;
};

#endif // __AUTHOR__