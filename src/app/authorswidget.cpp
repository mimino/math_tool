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
	
    //readFromFile1("f:\Papers.txt");
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

void AuthorsWidget::readFromFile(QString fileName)
{
    //table->readFromFile(fileName);

	QFile papers;
	papers.setFileName("f:\Papers.txt");
	if (!papers.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}

	QSqlTableModel* model = new QSqlTableModel(this);
	model->setTable("authors");
	//model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	//model->select();

	long long i=8;
	//QList<QByteArray> strings;
	while (!papers.atEnd()) {
		QByteArray line = papers.readLine();
		//strings += line.split('\t');
		QString queryStr = QString("insert into authors values(%1, '%2', %3)").arg(i).arg(QString(line)).arg('YY');
		//bool res = query.exec(queryStr);

		QSqlRecord record;

		QSqlField f1("id", QVariant::Int);
		QSqlField f2("author", QVariant::String);
		QSqlField f3("organization", QVariant::String);

		f1.setValue(QVariant(i));
		f2.setValue(QVariant(QString(line)));
		f3.setValue(QVariant("YY"));

		record.append(f1);
		record.append(f2);
		record.append(f3);

		model->insertRecord(-1, record);
		//model->submitAll();
		i++;
	}


	int a = 0;
	a++;
}

void AuthorsWidget::readFromFile1(QString fileName)
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	int i = 0;
	QTextStream stream ( &file );
	QString line;
	//QSqlQuery q("INSERT INTO authors (id,name,address) VALUES (:id,:name,:address)");

	QSqlTableModel* model = new QSqlTableModel(this);
	model->setTable("authors");

	QSqlQuery q;
	qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz");
	QStringList lines;
	int k=0;
	while(!stream.atEnd())
	{
		line = stream.readLine();
		lines.append(line);
		i++;

//		QString queryStr = QString("insert into authors values(%1,'%2','%3')").arg(i).arg(line).arg("NN");
//		q.exec(queryStr);

		if(i > 2000000)
		{
			model->database().transaction();
			int k0=k;
			for(k=k0; k<k0+2000000; k++)
			{
				QString queryStr = QString("insert into authors values(%1,'%2','%3')").arg(k).arg(lines.at(k-k0)).arg("NN");
				q.exec(queryStr);
			}

			model->database().commit();

			i=0;
			lines.clear();
			//break;
		}
	}
	
	qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz");

	file.close(); // when your done.
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


