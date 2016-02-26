#ifndef JOURNALSWIDGET_H
#define JOURNALSWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class JournalsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JournalsWidget(QAbstractItemModel* model, QWidget *parent = 0);

signals:

public slots:

};

#endif // JOURNALSWIDGET_H
