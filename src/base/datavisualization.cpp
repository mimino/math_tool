#include <QFile>
#include <igraph.h>
#include "datavisualization.h"


DataVisualization::DataVisualization(QObject* parent, DataModel& model, int entityType, int subjectId) : QObject(parent), _dataModel(model), _entityType(entityType), _subjectId(subjectId)
{ 
	init();
}

DataVisualization::~DataVisualization()
{
	reset();
}

void DataVisualization::init()
{
	initBasis();
	initAuthors();
}

void DataVisualization::initBasis()
{
	QString basisCoordinatesFN = QString("%1data/basis-coordinates-%2").arg(APPLICATION_PATH).arg(_subjectId);
	if(QFile::exists(basisCoordinatesFN))
	{
		restoreBasisCoordinates();
	}
	else
	{
		calculateBasisCoordinates();
	}
}

void DataVisualization::initAuthors()
{
	_authorsCoordinates.clear(); 
	_authorsSizes.clear(); 
	_authorsGraph.clear();

	for(int i=0; i<_dataModel._authors.count(); i++)
	{
		Author author = _dataModel._authors.at(i);
		float x = 0, y = 0, z = 0;
		for(int k=0; k<author._keywords.count(); k++)
		{
			x += _basisCoordinates.at(author._keywords.at(k)).x()/author._keywords.count();
			y += _basisCoordinates.at(author._keywords.at(k)).y()/author._keywords.count();
			z += _basisCoordinates.at(author._keywords.at(k)).z()/author._keywords.count();
		}

		_authorsCoordinates.append(Point(x, y, z));

		_authorsSizes.append(author._weight);

		for(int k=0; k<author._coauthors.count(); k++)
		{
			QVector<int> v;
			v.append(i);
			v.append(author._coauthors.at(k));
			_authorsGraph.append(Edge(v));
		}
	}
}

void DataVisualization::reset()
{
	_basisCoordinates.clear(); 
	_authorsCoordinates.clear(); 
	_authorsSizes.clear(); 
	_authorsGraph.clear();
}

PointsList& DataVisualization::coordinates()
{
	if(_entityType == ENTITY_TYPE_AUTHORS)
	{
		return _authorsCoordinates;
	}
	else if(_entityType == ENTITY_TYPE_PUBLICATIONS)
	{
		return _publicationsCoordinates;
	}
	else if(_entityType == ENTITY_TYPE_KEYWORDS)
	{
		return _basisCoordinates;
	}
}

QVector<int>& DataVisualization::sizes()
{
	if(_entityType == ENTITY_TYPE_AUTHORS)
	{
		return _authorsSizes;
	}
	else if(_entityType == ENTITY_TYPE_PUBLICATIONS)
	{
		return _publicationsSizes;
	}
}

EdgesList& DataVisualization::graph()
{
	if(_entityType == ENTITY_TYPE_AUTHORS)
	{
		return _authorsGraph;
	}
	else if(_entityType == ENTITY_TYPE_PUBLICATIONS)
	{
		return _publicationsGraph;
	}
}

void DataVisualization::restoreBasisCoordinates()
{
    int zeroTen = 0;
    int tenHundred = 0;
    int hundredFiveHundred = 0;
    int fiveHundredThousand = 0;
    int others = 0;

    for(int i=0; i<_dataModel._keywords.count(); i++)
    {
        if(_dataModel._keywords[i]._weight > 0 && _dataModel._keywords[i]._weight <= 10)
        {
            zeroTen++;
        }
        else if(_dataModel._keywords[i]._weight > 10 && _dataModel._keywords[i]._weight <= 100)
        {
            tenHundred++;
        }
        else if(_dataModel._keywords[i]._weight > 100 && _dataModel._keywords[i]._weight <= 500)
        {
            hundredFiveHundred++;
        }
        else if(_dataModel._keywords[i]._weight > 500 && _dataModel._keywords[i]._weight <= 1000)
        {
            fiveHundredThousand++;
        }
        else if(_dataModel._keywords[i]._weight > 1000 && _dataModel._keywords[i]._weight <= 10000)
        {
            others++;
        }
    }

    _basisCoordinates.clear();

    QList<QByteArray> strings;
	QString coordinatesFN = QString("%1data/basis-coordinates-%2").arg(APPLICATION_PATH).arg(_subjectId);
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
    _basisCoordinates.append(Point(0, 0, 0));

    for(int i=1; i<_dataModel._keywords.count(); i++)
    {
        float xi = strings[3*i].toFloat();
        float yi = strings[3*i+1].toFloat();
        float zi = strings[3*i+2].left(strings[3*i+2].length()-1).toFloat();
        _basisCoordinates.append(Point(xi - x0,
                                  yi - y0,
                                  zi - z0));
    }
}

int DataVisualization::mapWeightToGraphPointSize(int weight)
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

void DataVisualization::calculateBasisCoordinates()
{
	igraph_t g;
	FILE* f;
	igraph_matrix_t coords;
	/* long int i, n; */

	QString keywordsRelationsFN = QString("%1data/keywords-relations-%2").arg(APPLICATION_PATH).arg(_subjectId);
	f=fopen(keywordsRelationsFN.toStdString().c_str(), "r");
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

	_basisCoordinates.clear();

	QString coordinatesFN = QString("%1data/basis-coordinates-%2").arg(APPLICATION_PATH).arg(_subjectId);
	QFile coordinatesFile;
	coordinatesFile.setFileName(coordinatesFN);
	if (!coordinatesFile.open(QIODevice::WriteOnly)) {
		return;
	}

	QByteArray data;
	data.clear();
	for(int i=0; i<nVert; i++)
	{
		float x = MATRIX(coords, i, 0);
		float y = MATRIX(coords, i, 1);
		float z = MATRIX(coords, i, 2);
		
		QString line;
		line.append(QString("%1 %2 %3").arg(x).arg(y).arg(z));
		line.append("\n");
		
		data.append(line);


		_basisCoordinates.append(Point(x, y, z));
	}

	coordinatesFile.write(data);
	coordinatesFile.flush();
	coordinatesFile.close();
		
	igraph_matrix_destroy(&coords);
	igraph_destroy(&g);
	fclose(f);
}
