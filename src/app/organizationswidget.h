#ifndef ORGANIZATIONSWIDGET_H
#define ORGANIZATIONSWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class OrganizationsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OrganizationsWidget(QAbstractItemModel* model, QWidget *parent = 0);

signals:

public slots:

};

#endif // ORGANIZATIONSWIDGET_H
