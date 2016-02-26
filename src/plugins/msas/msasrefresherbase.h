#ifndef MSASREFRESHERBASE_H
#define MSASREFRESHERBASE_H

#include <QObject>

class MSASRefresherBase : public QObject
{
    Q_OBJECT
public:
    explicit MSASRefresherBase(QObject *parent = 0);
    virtual ~MSASRefresherBase();
    virtual void refresh();

signals:
    void refreshFinished();
};

#endif // MSASREFRESHERBASE_H
