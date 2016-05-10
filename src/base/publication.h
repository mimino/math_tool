#ifndef __PUBLICATION__
#define __PUBLICATION__

#include <QString>
#include "common_stuff.h"

class MATHTOOL_EXPORT Publication
{
public:
    Publication();
	Publication(QString title, QString url, int weight);
	bool isInitialized() { return _title.length() != 0; }
public:
	QString _title;
	QString _url;
	int _weight;
};

#endif // __PUBLICATION__