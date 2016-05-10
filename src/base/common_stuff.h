#ifndef __COMMON_STUFF__
#define __COMMON_STUFF__

#include <qglobal.h>

#define VERSION "1.1.0"

#if defined(MATHTOOL_LIBRARY)
#  define MATHTOOL_EXPORT Q_DECL_EXPORT
#else
#  define MATHTOOL_EXPORT Q_DECL_IMPORT
#endif

#define APPLICATION_PATH "../build/"
#define PLUGINS_PATH "../build/plugins/"

#endif // __COMMON_STUFF__
