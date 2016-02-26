#ifndef MSASENTRY_H
#define MSASENTRY_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QDomElement>

class MSASEntry
{
public:
    MSASEntry();
    ~MSASEntry();
public:
    void parse(QDomElement& element);
    void saveFull();
private:
    void parseAuthors(QString& text);
    void parseSubjects(QString& text);
private:
    QList<QString> _authors;
    QString _title;
    QList<QString> _subjects;
    QList<QString> _journals;
};

#endif // MSASENTRY_H
