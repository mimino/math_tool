#ifndef __ARXIV_PARSER_H__
#define __ARXIV_PARSER_H__

#include <QtGui/QMainWindow>
#include <QtGui/QScrollArea>
#include <QtGui/QFrame>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QFormLayout>
#include <QtGui/QMessageBox>

#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

#include <QtXml/QXmlStreamReader>
#include <QDomElement>

class ArxivEntry;

class ArxivParser : public QMainWindow
{
    Q_OBJECT

public:
    ArxivParser(QWidget *parent = 0);
    ~ArxivParser();

private slots:
    void parseXML();

private:
    QPointer<QVBoxLayout> _layout;
    void setupUI();

    void handleDomElement(QDomElement& elem);
    void doParseXML(QString filename);
private:
    QList<ArxivEntry> _entries;
};

#endif // __ARXIV_PARSER_H__
