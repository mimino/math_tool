#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QTabWidget>

class ContentWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ContentWidget(QWidget *parent = 0);

private:
    void setupTabs();

};

#endif // CONTENTWIDGET_H
