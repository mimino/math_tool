#ifndef MSASENTRY_H
#define MSASENTRY_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QDomElement>

class MSASPublication
{
public:
    MSASPublication();
    ~MSASPublication();
public:
    bool isInitialized() { return _title.length() != 0; }
    void parse(QDomElement& element);
    //void saveFull();
private:
    //void parseAuthors(QString& text);
    //void parseSubjects(QString& text);
private:
    QString _title;
    QString _url;
};

#endif // MSASENTRY_H
