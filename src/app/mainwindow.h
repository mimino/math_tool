#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class ContentWidget;
class SubjectsWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void changeSubject(QModelIndex index);
    void refresh();

private:
    void createMenus();

private:
    ContentWidget* _contentWidget;
    SubjectsWidget* _subjectsWidget;

    QMenu* _fileMenu;
    QAction* _openAct;
    QAction* _saveAct;
    QAction* _exitAct;

};

#endif // MAINWINDOW_H
