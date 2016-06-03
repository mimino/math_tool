#ifndef __DATA_VISUALIZATION__
#define __DATA_VISUALIZATION__

#include <QObject>

#include "types.h"
#include "datamodel.h"
#include "common_stuff.h"

class MATHTOOL_EXPORT DataVisualization : public QObject
{
    Q_OBJECT
public:
	DataVisualization(QObject* parent, DataModel& model, int entityType, int subjectId = 0);
	~DataVisualization();

	PointsList& coordinates();
	QVector<int>& sizes();
	EdgesList& graph();

private:
	void init();
	void initBasis();
	void initAuthors();
	void reset();

	void restoreBasisCoordinates();
	int mapWeightToGraphPointSize(int weight);
	void calculateBasisCoordinates();
public:
	DataModel& _dataModel;
	int _entityType;
	int _subjectId;

	PointsList _authorsCoordinates;
	QVector<int> _authorsSizes;
	EdgesList _authorsGraph;

	PointsList _publicationsCoordinates;
	QVector<int> _publicationsSizes;
	EdgesList _publicationsGraph;

	PointsList _basisCoordinates; // Coordinates of keywords in 3D space
};

#endif // __DATA_VISUALIZATION__