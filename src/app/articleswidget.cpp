#include <QtWidgets>
#include <QtSql>
#include "articleswidget.h"

ArticlesWidget::ArticlesWidget(QWidget *parent) :
    QWidget(parent)
{
    GlobalContext::addObserver(this);

    _model = new QSqlQueryModel();
    _model->setQuery("select p.* from publications as p "
                           "inner join publicationsubject as ps on p.id = ps.publicationid "
                           "inner join subjects as s on ps.subjectid = s.id "
                           "where ps.subjectid = 5"
                           );


    QPalette Pal(palette());

    // set black background
    Pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(Pal);

    _tableView = new QTableView;
    _tableView->setModel(_model);
    //_tableView->setModelColumn(1);
    _tableView->setSortingEnabled(true);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->verticalHeader()->hide();
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    //_tableView->hideColumn(0);
    //_tableView->horizontalHeader()->model()->setHeaderData(0, Qt::Horizontal, tr("Id"));
    //_tableView->horizontalHeader()->model()->setHeaderData(1, Qt::Horizontal, tr("Name"));
    //_tableView->horizontalHeader()->model()->setHeaderData(2, Qt::Horizontal, tr("Address"));

    _mainLayout = new QHBoxLayout;
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_tableView);
    setLayout(_mainLayout);
}

void ArticlesWidget::contextChanged()
{
    QString queryStr = QString("select p.* from publications as p "
                               "inner join publicationsubject as ps on p.id = ps.publicationid "
                               "inner join subjects as s on ps.subjectid = s.id "
                               "where ps.subjectid = %1").arg(GlobalContext::getCurrentSubjectId());
    _model->setQuery(queryStr);
}
