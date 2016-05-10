#ifndef __REG_EXP__
#define __REG_EXP__

#include <QString>
#include "common_stuff.h"

class MATHTOOL_EXPORT RegExp
{
public:
    static QString getByPattern(const QString& pattern, const QString& text);
};


#endif // __REG_EXP__
