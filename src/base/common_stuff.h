#ifndef __COMMON_STUFF_H__
#define __COMMON_STUFF_H__

#include <qglobal.h>

#define VERSION "1.1.0"

#if defined(MATHTOOL_LIBRARY)
#  define MATHTOOL_EXPORT Q_DECL_EXPORT
#else
#  define MATHTOOL_EXPORT Q_DECL_IMPORT
#endif

#endif
