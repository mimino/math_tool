#ifndef __MSAS_PLUGIN_H__
#define __MSAS_PLUGIN_H__

#include <QObject>
#include <QtCore/qplugin.h>
#include "../../base/plugin_interface.h"
#include "httprequest.h"
#include "msasrefresherbase.h"


class MSASPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.mathtool.mathtool.plugin/1.0" FILE "msas.json")
    Q_INTERFACES(PluginInterface)

public:
    MSASPlugin();
    ~MSASPlugin();
private:
    QString name() Q_DECL_OVERRIDE;
    int refresh() Q_DECL_OVERRIDE;
    void switchState();
private slots:
    void finished();

private:
    enum STATE
    {
        eIdle = 0,
        ePublications,
        eAuthors
    } _currentState;

    MSASRefresherBase* _refresher;
};


#endif // __MSAS_PLUGIN_H__
