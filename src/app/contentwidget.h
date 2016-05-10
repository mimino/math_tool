#ifndef __CONTENT_WIDGET__
#define __CONTENT_WIDGET__

#include <QTabWidget>

class ContentWidget : public QTabWidget
{
	Q_OBJECT
public:
	explicit ContentWidget(QWidget *parent = 0);

private:
	void setupTabs();

};

#endif // __CONTENT_WIDGET__
