#ifndef __AUTHORS_WIDGET_H__
#define __AUTHORS_WIDGET_H__

#include "globalcontext.h"
#include "extendedtabwidget.h"
#include "tablemodel.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
class QSqlQueryModel;
class QItemSelection;
class QTableView;
QT_END_NAMESPACE


class AuthorsWidget : public QWidget, public GlobalContextObserver
{
    Q_OBJECT
public:
    explicit AuthorsWidget(QWidget *parent = 0);
    void readFromFile(QString fileName);
public slots:
    void addEntry();
    void addEntry(QString name, QString address);
    void editEntry();
    void removeEntry();

//signals:
//    void selectionChanged (const QItemSelection &selected);

    void setCurrentIndex(int index);
private:
    void setupTabs();
    void contextChanged();
private:
    QButtonGroup* _buttonGroup;
    QHBoxLayout* _mainLayout;
    QVBoxLayout* _buttonLayout;
    QTableView* _tableView;
    int _currentIndex;
    QSortFilterProxyModel *proxyModel;
    QStringList _groups;
    QSqlQueryModel* _model;
};

#endif // __AUTHORS_WIDGET_H__
