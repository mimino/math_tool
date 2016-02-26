#ifndef __ARXIV_PLUGIN_H__
#define __ARXIV_PLUGIN_H__

//#include <QObject>
#include <QtCore/qplugin.h>
#include "../../base/plugin_interface.h"

class ArxivPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.mathtool.mathtool.plugin/1.0" FILE "arxiv.json")
    Q_INTERFACES(PluginInterface)

public:
	ArxivPlugin();
	~ArxivPlugin();
private:
    QString name() Q_DECL_OVERRIDE;
    int refresh() Q_DECL_OVERRIDE;
};

#endif // __ARXIV_PLUGIN_H__
