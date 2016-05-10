#ifndef __ORGANIZATIONS_WIDGET__
#define __ORGANIZATIONS_WIDGET__

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

#endif // __ORGANIZATIONS_WIDGET__
