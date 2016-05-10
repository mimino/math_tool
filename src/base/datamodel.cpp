#include <QDir>
#include <QPluginLoader>
#include <QtGlobal>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <igraph.h>

#include "plugin_interface.h"
#include "datamodel.h"


DataModel::DataModel(QObject *parent) : QObject(parent)
{
	QDir dir;
	dir.cd(APPLICATION_PATH);
	dir.mkdir("data");
}

void DataModel::refresh(int subjectId)
{
    _curSubjectId = subjectId;
	QString keywordsFN = QString("%1data/keywords-%2").arg(APPLICATION_PATH).arg(subjectId);
	QString graphFN = QString("%1data/graph-%2").arg(APPLICATION_PATH).arg(subjectId);
	if(QFile::exists(keywordsFN) && QFile::exists(graphFN))
	{
		refreshFromLocal(subjectId);
	}
	else
	{
		refreshFromRemote(subjectId);
	}
}

void DataModel::refreshFromLocal(int subjectId)
{
	QString keywordsFN = QString("%1data/keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
    QFile keywordsFile;
    keywordsFile.setFileName(keywordsFN);
    if (!keywordsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

	QList<QByteArray> strings;
    while (!keywordsFile.atEnd()) {
        QByteArray line = keywordsFile.readLine();
        strings += line.split('|');
    }

	for(int i=0; i<strings.count(); i+=2)
	{
		_keywords.append(Keyword(strings[i], "", QString(strings[i+1]).toInt()));
	}

	keywordsFile.close();


	QString graphFN = QString("%1data/graph-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
    QFile graphFile;
    graphFile.setFileName(graphFN);
	if (!graphFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

	strings.clear();
    while (!graphFile.atEnd()) {
        QByteArray line = graphFile.readLine();
        strings += line.split(' ');
    }

	for(int i=0; i<strings.count(); i+=2)
	{
		int p0 = QString(strings[i]).toInt();
		int p1 = QString(strings[i+1]).toInt();
		QPair<int, int> pair(p0, p1);
		_keywordsRelations.append(pair);
	}

	graphFile.close();

	refreshGraph();
}

void DataModel::refreshFromRemote(int subjectId)
{
    QDir pluginsDir(PLUGINS_PATH);
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            PluginInterface* pluginInterface = qobject_cast<PluginInterface*>(plugin);
            if(pluginInterface)
            {
                connect(dynamic_cast<QObject*>(pluginInterface), SIGNAL(pluginRefreshFinished(PluginInterface*)), SLOT(refreshFromRemoteFinished(PluginInterface*)));
                pluginInterface->refresh(subjectId);
            }
        }
    }
}

void DataModel::refreshFromRemoteFinished(PluginInterface* plugin)
{
    plugin->getKeywords(_curSubjectId, _keywords, _keywordsRelations);

	storeToLocal();

	placeGraph();
	refreshGraph();
}

void DataModel::refreshGraph()
{
    int zeroTen = 0;
    int tenHundred = 0;
    int hundredFiveHundred = 0;
    int fiveHundredThousand = 0;
    int others = 0;

    for(int i=0; i<_keywords.count(); i++)
    {
        if(_keywords[i]._weight > 0 && _keywords[i]._weight <= 10)
        {
            zeroTen++;
        }
        else if(_keywords[i]._weight > 10 && _keywords[i]._weight <= 100)
        {
            tenHundred++;
        }
        else if(_keywords[i]._weight > 100 && _keywords[i]._weight <= 500)
        {
            hundredFiveHundred++;
        }
        else if(_keywords[i]._weight > 500 && _keywords[i]._weight <= 1000)
        {
            fiveHundredThousand++;
        }
        else if(_keywords[i]._weight > 1000 && _keywords[i]._weight <= 10000)
        {
            others++;
        }
    }

    _graph.reset();
	
	QVector<int> edge;
	for(int i=0; i<_keywordsRelations.count(); i++)
    {
		edge.clear();
		int a = _keywordsRelations[i].first;
		int b = _keywordsRelations[i].second;

		edge.append(a);
        edge.append(b);
        _graph._complex.append(Edge(edge));
    }

    QList<QByteArray> strings;
	QString coordinatesFN = QString("%1data/coordinates-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
    QFile input;
    input.setFileName(coordinatesFN);

    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!input.atEnd()) {
        QByteArray line = input.readLine();
        strings += line.split(' ');
    }

    float x0 = strings[0].toFloat();
    float y0 = strings[1].toFloat();
    float z0 = strings[2].left(strings[2].length()-1).toFloat();
    _graph._coordinates.append(Point(0, 0, 0));

    for(int i=1; i<_keywords.count(); i++)
    {
        float xi = strings[3*i].toFloat();
        float yi = strings[3*i+1].toFloat();
        float zi = strings[3*i+2].left(strings[3*i+2].length()-1).toFloat();
        _graph._coordinates.append(Point(xi - x0,
                                  yi - y0,
                                  zi - z0));
    }

    for(int i=0; i<_keywords.count(); i++)
    {
        _graph._sizes.append(mapWeightToGraphPointSize(_keywords[i]._weight));
    }
}

Graph& DataModel::getGraph()
{
    return _graph;
}

int DataModel::mapWeightToGraphPointSize(int weight)
{
    int res = 1;
    if(weight > 0 && weight <= 10)
    {
        res = 2;
    }
    else if(weight > 10 && weight <= 100)
    {
        res = 10;
    }
    else if(weight > 100 && weight <= 500)
    {
        res = 20;
    }
    else if(weight > 500 && weight <= 1000)
    {
        res = 50;
    }
    else if(weight > 1000 && weight <= 10000)
    {
        res = 100;
    }

    return res;
}

void DataModel::storeToLocal()
{
	QString keywordsFN = QString("%1data/keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
    QFile keywordsFile;
    keywordsFile.setFileName(keywordsFN);
    if (!keywordsFile.open(QIODevice::WriteOnly)) {
        return;
    }

    QByteArray data;
    for(int i=0; i<_keywords.count(); i++)
    {
		QString line = QString("%1|%2\n").arg(_keywords[i]._title).arg(_keywords[i]._weight);
		data.append(line);
    }

    keywordsFile.write(data);
    keywordsFile.flush();
    keywordsFile.close();


	QString graphFN = QString("%1data/graph-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
    QFile graphFile;
    graphFile.setFileName(graphFN);
    if (!graphFile.open(QIODevice::WriteOnly)) {
        return;
    }

	data.clear();
    for(int i=0; i<_keywordsRelations.count(); i++)
    {
		QString line;
        line.append(QString("%1").arg(_keywordsRelations[i].first));
        line.append(" ");
        line.append(QString("%1").arg(_keywordsRelations[i].second));
        line.append("\n");

		data.append(line);
    }

    graphFile.write(data);
    graphFile.flush();
    graphFile.close();
}

void DataModel::placeGraph()
{
	igraph_t g;
	FILE* f;
	igraph_matrix_t coords;
	/* long int i, n; */

	QString graphFN = QString("%1data/graph-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	f=fopen(graphFN.toStdString().c_str(), "r");
	igraph_read_graph_edgelist(&g, f, 0, 1);
	igraph_matrix_init(&coords, 0, 0);
	//igraph_layout_reingold_tilford(&g, &coords, IGRAPH_ALL, 0, 0); 
	igraph_layout_fruchterman_reingold_3d(&g, &coords, 500, 1000, igraph_vcount(&g)^3, 1.5, igraph_vcount(&g)^3, false, 0, 0, 0, 0, 0, 0, 0); 

	int nVert=igraph_vcount(&g);
	igraph_integer_t from, to, eid;

	/*FILE* f2=fopen("graph.txt", "a");
	int nEdge=igraph_ecount(&_g);
	for (int i=0; i<nEdge; i++) 
	{
		igraph_edge(&_g, i, &from, &to);
		fprintf(f2, "%d %d %d\n", from, to, 2812);
	}

	fflush(f2);
	fclose(f2);
	*/

	QString coordinatesFN = QString("%1data/coordinates-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile coordinatesFile;
	coordinatesFile.setFileName(coordinatesFN);
	if (!coordinatesFile.open(QIODevice::WriteOnly)) {
		return;
	}

	QByteArray data;
	data.clear();
	for(int i=0; i<nVert; i++)
	{
		QString line;
		line.append(QString("%1").arg(MATRIX(coords, i, 0)));
		line.append(" ");
		line.append(QString("%1").arg(MATRIX(coords, i, 1)));
		line.append(" ");
		line.append(QString("%1").arg(MATRIX(coords, i, 2)));
		line.append("\n");

		data.append(line);
	}

	coordinatesFile.write(data);
	coordinatesFile.flush();
	coordinatesFile.close();

	
	igraph_matrix_destroy(&coords);
	igraph_destroy(&g);
	fclose(f);
}
