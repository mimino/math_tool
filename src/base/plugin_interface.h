#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QtCore/qplugin.h>
#include <QString>
#include <QIcon>

class PluginInterface
{
public:
	virtual ~PluginInterface() {}

	virtual QString name() = 0;
	virtual int refresh() = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.mathtool.mathtool.plugin/1.0")

#endif // PLUGIN_INTERFACE_H
