#ifndef __DATA_MODEL__
#define __DATA_MODEL__

#include "common_stuff.h"
#include "author.h"
#include "publication.h"
#include "journal.h"
#include "organization.h"
#include "keyword.h"
#include "plugin_interface.h"
#include "types.h"

class MATHTOOL_EXPORT DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = 0);
    void refresh(int subjectId = 0);
    Graph& getGraph();
private:
	void refreshFromLocal(int subjectId = 0);
    void refreshFromRemote(int subjectId = 0);
    void refreshGraph();
    int mapWeightToGraphPointSize(int weight);
	void storeToLocal();
	void placeGraph();
private slots:
    void refreshFromRemoteFinished(PluginInterface* plugin);
public:
    int _curSubjectId;
    QList<Publication> _publications;
    QList<Author> _authors;
    QList<Keyword> _keywords;
    QList<QPair<int, int> > _keywordsRelations;
    QList<Journal> _journals;
    QList<Organization> _organizations;
    Graph _graph;
};

#endif // __DATA_MODEL__