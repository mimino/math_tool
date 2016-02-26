/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtSql>

#include "configdialog.h"
#include "pages.h"
#include "authorswidget.h"

ConfigDialog::ConfigDialog() : QMainWindow()
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable("articles");
    _model->setRelation(2, QSqlRelation("authors", "id", "author"));
    _model->select();

    _contentsWidget = new QListWidget;
    _contentsWidget->setViewMode(QListView::ListMode);
    _contentsWidget->setIconSize(QSize(40, 40));
    _contentsWidget->setMovement(QListView::Static);
    _contentsWidget->setMaximumWidth(140);
    //contentsWidget->setSpacing(5);

    _pagesWidget = new QStackedWidget;
    _authorsWidget = new AuthorsWidget();
    _pagesWidget->addWidget(_authorsWidget);
    //pagesWidget->addWidget(createAuthorGroupBox());

    _articleView = new QTableView;
    _articleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _articleView->setSortingEnabled(true);
    _articleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _articleView->setSelectionMode(QAbstractItemView::SingleSelection);
    _articleView->setShowGrid(false);
    _articleView->verticalHeader()->hide();
    _articleView->setAlternatingRowColors(true);
    _articleView->setModel(_model);

    _pagesWidget->addWidget(_articleView);
    _pagesWidget->addWidget(new QueryPage);
    //_pagesWidget->addWidget(new QueryPage);
    //_pagesWidget->addWidget(new QueryPage);

    QPushButton *closeButton = new QPushButton(tr("Close"));

    createIcons();
    _contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(_contentsWidget);
    horizontalLayout->addWidget(_pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    createMenus();

    resize(850, 400);
}

QGroupBox* ConfigDialog::createAuthorGroupBox()
{
    _authorView = new QTableView;
    _authorView->setModel(_model->relationModel(2));
    _authorView->setSortingEnabled(true);
    _authorView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _authorView->setSelectionMode(QAbstractItemView::SingleSelection);
    _authorView->setShowGrid(false);
    _authorView->verticalHeader()->hide();

    //authorView->hideColumn(0);
    //authorView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    //authorView->resizeColumnToContents(2);
    //authorView->resizeColumnToContents(3);

    //authorView->setModelColumn(1);

    //connect(authorView, SIGNAL(clicked(QModelIndex)),
    //        this, SLOT(changeAuthor(QModelIndex)));
    //connect(authorView, SIGNAL(activated(QModelIndex)),
    //        this, SLOT(changeAuthor(QModelIndex)));

    QGroupBox *box = new QGroupBox(tr("Author"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(_authorView, 0, 0);
    box->setLayout(layout);

    return box;
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *authorsButton = new QListWidgetItem(_contentsWidget);
    authorsButton->setSizeHint(QSize(140, 50));
    authorsButton->setIcon(QIcon(":/images/config.png"));
    authorsButton->setText(tr("Authors"));
    authorsButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    authorsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *articlesButton = new QListWidgetItem(_contentsWidget);
    articlesButton->setSizeHint(QSize(140, 50));
    articlesButton->setIcon(QIcon(":/images/update.png"));
    articlesButton->setText(tr("Articles"));
    articlesButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    articlesButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *subjectsButton = new QListWidgetItem(_contentsWidget);
    subjectsButton->setSizeHint(QSize(140, 50));
    subjectsButton->setIcon(QIcon(":/images/query.png"));
    subjectsButton->setText(tr("Subjects"));
    subjectsButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    subjectsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *keywordsButton = new QListWidgetItem(_contentsWidget);
    keywordsButton->setSizeHint(QSize(140, 50));
    keywordsButton->setIcon(QIcon(":/images/query.png"));
    keywordsButton->setText(tr("Keywords"));
    keywordsButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    keywordsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *organizationButton = new QListWidgetItem(_contentsWidget);
    organizationButton->setSizeHint(QSize(140, 50));
    organizationButton->setIcon(QIcon(":/images/query.png"));
    organizationButton->setText(tr("Organizations"));
    organizationButton->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    organizationButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(_contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::createMenus()
{
    _fileMenu = menuBar()->addMenu(tr("&File"));

    _openAct = new QAction(tr("&Open..."), this);
    _fileMenu->addAction(_openAct);
    connect(_openAct, SIGNAL(triggered()),
        this, SLOT(openFile()));

    _saveAct = new QAction(tr("&Save As..."), this);
    _fileMenu->addAction(_saveAct);
    connect(_saveAct, SIGNAL(triggered()),
        this, SLOT(saveFile()));

    _fileMenu->addSeparator();

    _exitAct = new QAction(tr("E&xit"), this);
    _fileMenu->addAction(_exitAct);
    connect(_exitAct, SIGNAL(triggered()),
        this, SLOT(close()));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    _pagesWidget->setCurrentIndex(_contentsWidget->row(current));

}

void ConfigDialog::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        _authorsWidget->readFromFile(fileName);
    }
}

void ConfigDialog::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()) {
        //authorsWidget->writeToFile(fileName);
    }
}
