#ifndef __PAGES__
#define __PAGES__

#include <QWidget>

class ConfigurationPage : public QWidget
{
public:
	ConfigurationPage(QWidget *parent = 0);
};

class QueryPage : public QWidget
{
public:
	QueryPage(QWidget *parent = 0);
};

class UpdatePage : public QWidget
{
public:
	UpdatePage(QWidget *parent = 0);
};

#endif // __PAGES__
