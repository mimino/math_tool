#ifndef __MSAS_PLUGIN_H__
#define __MSAS_PLUGIN_H__

#include <QObject>
#include <QtCore/qplugin.h>
#include "../../base/plugin_interface.h"
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
    QString dataDir() Q_DECL_OVERRIDE;
    int refresh(int subjectId = 0) Q_DECL_OVERRIDE;
    void getKeywords(int subjectId, QList<Keyword>& keywords, QList<QPair<int, int> >& relations) Q_DECL_OVERRIDE;
	void getAuthors(int subjectId, QList<Author>& authors) Q_DECL_OVERRIDE;

    void switchState();
    int subjectIdToSubdomain(int subjectId);
signals:
    void pluginRefreshFinished(PluginInterface* plugin);
private slots:
    void finished();

private:
    enum STATE
    {
        eIdle = 0,
        ePublications,
        eAuthors,
        eKeywords
    } _currentState;

    MSASRefresherBase* _refresher;
    int _currentSubjectId;
    QList<Keyword> _keywords;
    QList<QPair<int, int> > _keywordsRelations;
	QList<Author> _authors;
};


#endif // __MSAS_PLUGIN_H__
