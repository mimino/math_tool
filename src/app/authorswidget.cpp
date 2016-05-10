#include <QtWidgets>
#include <QtSql>
#include "authorswidget.h"

//#include "adddialog.h"

AuthorsWidget::AuthorsWidget(QWidget *parent) :
    QWidget(parent)
{
    GlobalContext::addObserver(this);

    _model = new QSqlQueryModel();
    _model->setQuery("select * from authors");

    _buttonGroup = new QButtonGroup;

    connect(_buttonGroup,  SIGNAL(buttonClicked(int)), this, SLOT(setCurrentIndex(int)));

    _buttonLayout = new QVBoxLayout();
    _buttonLayout->setSpacing(0);

    QVBoxLayout* buttonStretchLayout = new QVBoxLayout();
    buttonStretchLayout->setSpacing(0);
    buttonStretchLayout->addLayout(_buttonLayout);
    buttonStretchLayout->addStretch();

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
   // _tableView->horizontalHeader()->model()->setHeaderData(2, Qt::Horizontal, tr("Address"));

    _mainLayout = new QHBoxLayout;
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addLayout(buttonStretchLayout);
    _mainLayout->addWidget(_tableView);
    setLayout(_mainLayout);

    _groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";
    setupTabs();
}

void AuthorsWidget::addEntry()
{
/*    AddDialog aDialog;

    if (aDialog.exec()) {
        QString name = aDialog.nameText->text();
        QString address = aDialog.addressText->toPlainText();
    }
*/
}

void AuthorsWidget::addEntry(QString name, QString address)
{
}

void AuthorsWidget::editEntry()
{
/*    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index, i;
    QString name;
    QString address;
    int row = -1;

    foreach (index, indexes) {
        row = proxy->mapToSource(index).row();
        i = table->index(row, 0, QModelIndex());
        QVariant varName = table->data(i, Qt::DisplayRole);
        name = varName.toString();

        i = table->index(row, 1, QModelIndex());
        QVariant varAddr = table->data(i, Qt::DisplayRole);
        address = varAddr.toString();
    }
//! [4a]

//! [4b]
    AddDialog aDialog;
    aDialog.setWindowTitle(tr("Edit a Contact"));

    aDialog.nameText->setReadOnly(true);
    aDialog.nameText->setText(name);
    aDialog.addressText->setText(address);

    if (aDialog.exec()) {
        QString newAddress = aDialog.addressText->toPlainText();
        if (newAddress != address) {
            i = table->index(row, 1, QModelIndex());
            table->setData(i, newAddress, Qt::EditRole);
        }
    }
*/
}

void AuthorsWidget::removeEntry()
{
/*    QTableView *temp = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
    QItemSelectionModel *selectionModel = temp->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indexes) {
        int row = proxy->mapToSource(index).row();
        table->removeRows(row, 1, QModelIndex());
    }

    if (table->rowCount(QModelIndex()) == 0) {
        insertTab(0, newAddressTab, "Address Book");
    }
*/
}

void AuthorsWidget::setupTabs()
{
    for (int i = 0; i < _groups.size(); ++i)
    {
        QString str = _groups.at(i);
        QPushButton *button = new QPushButton(str);
        button->setCheckable(true);
        if(i==0)
        {
            button->setChecked(true);
        }

        _buttonGroup->addButton(button, i);
        _buttonLayout->addWidget(button);

        //addTab(tableView, str);
    }
}

//int VerticalTabWidget::currentIndex() const
//{
//    return _currentIndex;
//}

void AuthorsWidget::setCurrentIndex(int index)
{
    /*if(index < 0 || index >= _groups.count())
    {
        index = 0;
    }

    table->setSelection(_groups.at(index));
    _buttonGroup->button(index)->setChecked(true);
    _tableView->scrollToBottom();
    _tableView->scrollToTop();
    */
}

void AuthorsWidget::contextChanged()
{
    QString queryStr = QString("select a.* from authors as a "
                     "inner join authorpublication as ap on a.id = ap.authorid "
                     "inner join publicationsubject as ps on ap.publicationid = ps.publicationid "
                     "inner join subjects as s on ps.subjectid = s.id "
                     "where ps.subjectid = %1").arg(GlobalContext::getCurrentSubjectId());
    _model->setQuery(queryStr);
}


