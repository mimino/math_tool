#include <QtWidgets>
#include <QtSql>

#include "globalcontext.h"
#include "subjectswidget.h"


SubjectsWidget::SubjectsWidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(10, 10);
    setMaximumWidth(120);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(Pal);

    _model = new QSqlTableModel();
    _model->setTable("subjects");

    _tableView = new QTableView;
    _tableView->setModel(_model);
    _tableView->setSortingEnabled(true);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->horizontalHeader()->setStretchLastSection(true);
    //_tableView->horizontalHeader()->hide();
    //_tableView->verticalHeader()->hide();
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    _tableView->setColumnHidden(0, true);

    connect(_tableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(test(QModelIndex)));

    _mainLayout = new QVBoxLayout;
    //_mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_tableView);

    setLayout(_mainLayout);
}

void SubjectsWidget::test(QModelIndex index)
{
    QSqlRecord record = _model->record(index.row());
    int subjectId = record.value("id").toInt();
    QString subject = record.value("subject").toString();
    GlobalContext::setCurrentSubjectId(subjectId);
}

