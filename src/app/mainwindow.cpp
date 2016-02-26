#include <QtWidgets>
#include <QtSql>
#include <QModelIndex>

#include "mainwindow.h"
#include "contentwidget.h"
#include "subjectswidget.h"
#include "addresswidget.h"
#include "../base/plugin_interface.h"

MainWindow::MainWindow() : QMainWindow()
{
    _subjectsWidget = new SubjectsWidget();
    _contentWidget = new ContentWidget();

    QPushButton* refreshButton = new QPushButton(tr("Refresh"));
    QPushButton* closeButton = new QPushButton(tr("Close"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

    //QVBoxLayout* topButtonsLayout = new QVBoxLayout;
    //topButtonsLayout->addStretch(1);
    //topButtonsLayout->addWidget(refreshButton);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(_subjectsWidget);
    //horizontalLayout->addWidget(new AddressWidget());
    horizontalLayout->addWidget(_contentWidget, 1);

//    connect(_subjectsWidget, SIGNAL(clicked(QModelIndex)),
//            this, SLOT(changeSubject(QModelIndex)));

    QHBoxLayout* bottomButtonsLayout = new QHBoxLayout;
    bottomButtonsLayout->addStretch(1);
    bottomButtonsLayout->addWidget(closeButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(refreshButton, 0, Qt::AlignLeft);
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
    QDir pluginsDir(path);
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            PluginInterface* pluginInterface = qobject_cast<PluginInterface*>(plugin);
            if(pluginInterface)
            {
                pluginInterface->refresh();
            }
        }
    }
}
