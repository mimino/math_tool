#ifndef __SUBJECTS_WIDGET__
#define __SUBJECTS_WIDGET__

#include <QWidget>
#include <QModelIndex>


QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
class QSqlTableModel;
class QItemSelection;
class QTableView;
class QVBoxLayout;
QT_END_NAMESPACE

class SubjectsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SubjectsWidget(QWidget *parent = 0);

private slots:
    void test(QModelIndex index);

private:
    QTableView* _tableView;
    QVBoxLayout* _mainLayout;
    QSqlTableModel* _model;
};

#endif // __SUBJECTS_WIDGET__
