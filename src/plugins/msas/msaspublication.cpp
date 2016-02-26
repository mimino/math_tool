#include "msaspublication.h"
#include <QDomNodeList>
#include <QFile>
#include <QMessageBox>

MSASPublication::MSASPublication()
{
}

MSASPublication::~MSASPublication()
{
}

void MSASPublication::parse(QDomElement& element)
{
    QDomNodeList aList = element.elementsByTagName("a");
    int count = aList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode node = aList.at(i);
        QDomElement subelem = node.toElement();

        if(subelem.hasAttribute("id") && subelem.attribute("id").contains("ctl00_MainContent_ct"))
        {
            _url = subelem.attribute("href");
            _title = subelem.text();
        }
    }
}
