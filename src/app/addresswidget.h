

#ifndef __AUTHORS_WIDGET_H__
#define __AUTHORS_WIDGET_H__

#include <QTabWidget>
#include <QItemSelection>
#include "tablemodel.h"
//#include "newaddresstab.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE


class AddressWidget : public QTabWidget
{
    Q_OBJECT

public:
    AddressWidget(QWidget *parent=0);
    void readFromFile(QString fileName);
    void writeToFile(QString fileName);

public slots:
    void addEntry();
    void addEntry(QString name, QString address);
    void editEntry();
    void removeEntry();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();

    TableModel *table;
//    NewAddressTab *newAddressTab;
    QSortFilterProxyModel *proxyModel;
};

#endif //__AUTHORS_WIDGET_H__
