#ifndef __DIGEST__
#define __DIGEST__ 

#include <QString>
#include "common_stuff.h"

class MATHTOOL_EXPORT Digest
{
public:
	Digest();
	Digest(QString title, QString url, int weight);
	bool isInitialized() { return _title.length() != 0; }
public:
	QString _title;
	QString _url;
	int _weight;
};

#endif // __DIGEST__