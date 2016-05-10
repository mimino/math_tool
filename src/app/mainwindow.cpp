#include <QtWidgets>
#include <QtSql>
#include <QModelIndex>

#include "globalcontext.h"
#include "mainwindow.h"
#include "contentwidget.h"
#include "subjectswidget.h"
#include "glwidget.h"
#include "datamodel.h"

MainWindow::MainWindow() : QMainWindow()
{
    _dataMdl = new DataModel();
    _subjectsWidget = new SubjectsWidget();
    _contentWidget = new ContentWidget();

    QPushButton* refreshButton = new QPushButton(tr("Refresh"));
    QPushButton* visualizeButton = new QPushButton(tr("Visualize"));
    QPushButton* closeButton = new QPushButton(tr("Close"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(visualizeButton, SIGNAL(clicked()), this, SLOT(visualize()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(_subjectsWidget);
    horizontalLayout->addWidget(_contentWidget, 1);

    QHBoxLayout* bottomButtonsLayout = new QHBoxLayout;
    bottomButtonsLayout->addStretch(1);
    bottomButtonsLayout->addWidget(closeButton);

    QHBoxLayout* topButtonsLayout = new QHBoxLayout;
    //topButtonsLayout->addStretch(1);
    topButtonsLayout->addWidget(refreshButton, 0, Qt::AlignLeft);
    topButtonsLayout->addWidget(visualizeButton, 1, Qt::AlignLeft);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topButtonsLayout, 1);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addLayout(bottomButtonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    createMenus();

    resize(850, 400);
}

void MainWindow::createMenus()
{
    _fileMenu = menuBar()->addMenu(tr("&File"));

    _openAct = new QAction(tr("&Open..."), this);
    _fileMenu->addAction(_openAct);

    _saveAct = new QAction(tr("&Save As..."), this);
    _fileMenu->addAction(_saveAct);

    _fileMenu->addSeparator();

    _exitAct = new QAction(tr("E&xit"), this);
    _fileMenu->addAction(_exitAct);
    connect(_exitAct, SIGNAL(triggered()),
        this, SLOT(close()));
}

void MainWindow::changeSubject(QModelIndex index)
{
}

void MainWindow::refresh()
{
    QString path(qApp->applicationDirPath());
    _dataMdl->refresh(GlobalContext::getCurrentSubjectId());
}

void MainWindow::visualize()
{
    GLWidget *glwidget = new GLWidget(*_dataMdl);
    glwidget->show();
}
