#include "arxivparser.h"
#include "arxiventry.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>

ArxivParser::ArxivParser(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

ArxivParser::~ArxivParser()
{
    _entries.clear();
}

void ArxivParser::setupUI() {
    QFrame* frame = new QFrame(this);
    _layout = new QVBoxLayout;
    frame->setLayout(_layout);
    _layout->addWidget(new QLabel("<h1>QXSR Example</h1>"),
                       0,
                       Qt::AlignCenter);

    QPushButton* parseXML = new QPushButton("Parse XML from example.xml");
    _layout->addWidget(parseXML);
    connect(parseXML, SIGNAL(clicked()),this, SLOT(parseXML()));

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(frame);
    scrollArea->setWidgetResizable(true);

    setCentralWidget(scrollArea);
}

void ArxivParser::parseXML()
{
    int year=91;
    while(year != 14)
    {
        for(int i=1;i<13;i++)
        {
            QString filename = QString("%1").arg(year, 2, 10, QChar('0')) + QString("%1").arg(i, 2, 10, QChar('0'));
            doParseXML(filename);
        }

        year++;

        if(year == 100)
        {
            year = 0;
        }
    }
}

void ArxivParser::doParseXML(QString filename)
{
    /* We'll parse the example.xml */
    QFile* file = new QFile(filename);
    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open example.xml",
                              QMessageBox::Ok);
        return;
    }

    /* QDomDocument takes any QIODevice. as well as QString buffer*/
    QDomDocument doc("mydocument");
    if (!doc.setContent(file))
    {
        return;
    }

    //Get the root element
    QDomElement docElem = doc.documentElement();
    // get the node's interested in, this time only caring about person's
    QDomNodeList nodeList = docElem.childNodes();

    //Check each node one by one.
    for(int ii = 0;ii < nodeList.count(); ii++)
    {
        QDomElement el = nodeList.at(ii).toElement();
        handleDomElement(el);
    }
}

void ArxivParser::handleDomElement(QDomElement& elem)
{
    QDomNodeList list = elem.childNodes();
    int count = list.count();
    for(int i=0; i<list.count(); i++)
    {
        QDomNode node = list.at(i);
        QDomElement subelem = node.toElement();

        if(subelem.tagName() == "dd")
        {
            ArxivEntry entry;
            entry.parse(subelem);
            _entries.append(entry);
        }
        else
        {
            handleDomElement(subelem);
        }
    }

    for(int i=0; i<_entries.count(); i++)
    {
        _entries[i].saveFull();
    }
}
