#ifndef __TYPES__
#define __TYPES__

#include <QVector>
#include <QVector3D>
#include <QList>


template<int dim>
class TSimplex
{
public:
    int dimension() const { return dim; }
    const int* data() const { return _ah.data(); }
    int operator [](int i) const { return _ah[i]; }
    void set(QVector<int>& args) {Q_ASSERT(args.length() == dim + 1); _ah = args;}
protected:
    TSimplex() {}
    TSimplex(QVector<int>& args) { set(args); }

private: // DATA
    QVector<int> _ah;
};

#define SIMPLEX_MARK_NONE 0
#define SIMPLEX_MARK_RED 1
#define SIMPLEX_MARK_BLUE 2

template <int dim>
class TMarkedSimplex : public TSimplex<dim>
{
public:
    TMarkedSimplex() : TSimplex<dim>() {};
    TMarkedSimplex(QVector<int>& args) : TSimplex<dim>(args) {};
public:
    int _mark;
};


class Vertex : public TMarkedSimplex<0>
{
public:
    Vertex() : TMarkedSimplex<0>() {};
    Vertex(int index) : TMarkedSimplex<0>() { QVector<int> v; v.append(index); set(v); }
};

typedef QVector3D Point;
typedef QVector<Point> PointsList;
//typedef TMarkedSimplex<0> Vertex;
typedef QVector<Vertex> VerticesList;
typedef TMarkedSimplex<1> Edge;
typedef QVector<Edge> EdgesList;

#define ENTITY_TYPE_AUTHORS 0
#define ENTITY_TYPE_PUBLICATIONS 1
#define ENTITY_TYPE_KEYWORDS 2

#endif // __TYPES__
