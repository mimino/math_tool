#ifndef ARTICLESWIDGET_H
#define ARTICLESWIDGET_H

#include <QWidget>
#include "globalcontext.h"

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
class QTableView;
class QHBoxLayout;
QT_END_NAMESPACE

class ArticlesWidget : public QWidget, public GlobalContextObserver
{
    Q_OBJECT
public:
    explicit ArticlesWidget(QWidget *parent = 0);

private:
    void contextChanged();
private:
    QTableView* _tableView;
    QHBoxLayout* _mainLayout;

    QSqlQueryModel* _model;
};

#endif // ARTICLESWIDGET_H
