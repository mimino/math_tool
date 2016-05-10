#ifndef __PLUGIN_INTERFACE__
#define __PLUGIN_INTERFACE__

#include <QtCore/qplugin.h>
#include <QString>
#include <QIcon>
#include "keyword.h"

class PluginInterface
{
public:
	virtual ~PluginInterface() {}

	virtual QString name() = 0;
	virtual QString dataDir() = 0;
    virtual int refresh(int subjectId = 0) = 0;
    virtual void getKeywords(int subjectId, QList<Keyword>& keywords, QList<QPair<int, int> >& relations) = 0;

signals:
    virtual void pluginRefreshFinished(PluginInterface* plugin) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.mathtool.mathtool.plugin/1.0")

#endif // __PLUGIN_INTERFACE__
