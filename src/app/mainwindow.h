#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class ContentWidget;
class SubjectsWidget;
class DataModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	private slots:
		void changeSubject(QModelIndex index);
		void refresh();
		void visualize();

private:
	void createMenus();

private:
	DataModel* _dataMdl;
	ContentWidget* _contentWidget;
	SubjectsWidget* _subjectsWidget;

	QMenu* _fileMenu;
	QAction* _openAct;
	QAction* _saveAct;
	QAction* _exitAct;

};

#endif // __MAIN_WINDOW__
