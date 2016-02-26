#ifndef ARXIVENTRY_H
#define ARXIVENTRY_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QDomElement>

class ArxivEntry
{
public:
    ArxivEntry();
    ~ArxivEntry();
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

#endif // ARXIVENTRY_H
