#include "arxiventry.h"
#include <QDomNodeList>
#include <QFile>
#include <QMessageBox>

ArxivEntry::ArxivEntry()
{
}

ArxivEntry::~ArxivEntry()
{
    _authors.clear();
    _subjects.clear();
}

void ArxivEntry::parse(QDomElement& element)
{
    QDomNode meta = element.firstChild();
    QDomNodeList list = meta.childNodes();
    int count = list.count();
    for(int i=0; i<count; i++)
    {
        QDomNode node = list.at(i);
        QDomElement subelem = node.toElement();

        if(subelem.tagName() == "div" && subelem.hasAttribute("class"))
        {
            if(subelem.attribute("class") == "list-title")
            {
                QString txt = subelem.text().mid(7);
                txt.replace("\n", " ");
                _title = txt.trimmed();
            }
            else if(subelem.attribute("class") == "list-authors")
            {
                QString txt = subelem.text();
                parseAuthors(txt);
            }
            else if(subelem.attribute("class") == "list-journal-ref")
            {
                QString txt = subelem.text();
                //journals.append(txt);
            }
            else if(subelem.attribute("class") == "list-subjects")
            {
                QString txt = subelem.text();
                parseSubjects(txt);
            }
        }
    }
}

void ArxivEntry::parseAuthors(QString& text)
{
    text.replace('\n', ' ');
    QString newText = text.mid(8); // Removing "Authors:"
    int endIndex = text.indexOf(")");
    while(endIndex != -1)
    {
        int startIndex = newText.left(endIndex).lastIndexOf("(");
        newText = newText.left(startIndex) + newText.mid(endIndex+1);
        endIndex = newText.indexOf(")");
    }

    int commaIndex = newText.indexOf(",");
    while(commaIndex != -1)
    {
        QString author = newText.left(commaIndex);
        _authors.append(author.trimmed());
        newText = newText.mid(commaIndex + 1);

        commaIndex = newText.indexOf(",");
    }

    if(newText.trimmed().length() != 0)
    {
        _authors.append(newText.trimmed());
    }
}

void ArxivEntry::parseSubjects(QString& text)
{
    text.replace("\n", "");
    QString subj;
    int endIndex = text.indexOf(")");
    while(endIndex != -1)
    {
        int startIndex = text.left(endIndex).lastIndexOf("(");
        subj = text.mid(startIndex + 1, endIndex - startIndex - 1);
        _subjects.append(subj.trimmed());
        text = text.mid(endIndex + 1);

        endIndex = text.indexOf(")");
    }

    if(text.trimmed().length() != 0)
    {
        _subjects.append(text.trimmed());
    }
}

void ArxivEntry::saveFull()
{
    QFile* fileToWriteAll = new QFile("All.txt");
    if (!fileToWriteAll->open(QIODevice::Append))
    {
        //QMessageBox::information(this, "HTTP",
        //                         "Unable to save the file %1: %2."
        //                         .arg("subjects.txt").arg(file->errorString()));
        delete fileToWriteAll;
        fileToWriteAll = 0;
        return;
    }

    for(int i=0; i<_authors.count(); i++)
    {
        for(int j=0; j<_subjects.count(); j++)
        {
            fileToWriteAll->write();
        }
    }


    fileToWriteAll->flush();
    fileToWriteAll->close();
    delete fileToWriteAll;
}

/*
    QFile* fileToWriteT = new QFile("titles.txt");
    if (!fileToWriteT->open(QIODevice::Append)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg("titles.txt").arg(file->errorString()));
        delete fileToWriteT;
        fileToWriteT = 0;
        return;
    }

    QFile* fileToWriteA = new QFile("authors.txt");
    if (!fileToWriteA->open(QIODevice::Append)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg("titles.txt").arg(file->errorString()));
        delete fileToWriteA;
        fileToWriteA = 0;
        return;
    }

    QFile* fileToWriteS = new QFile("subjects.txt");
    if (!fileToWriteS->open(QIODevice::Append))
    {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg("subjects.txt").arg(file->errorString()));
        delete fileToWriteS;
        fileToWriteS = 0;
        return;
    }

    int titlesCnt = titles.count();
    int authorsCnt = authors.count();

    if(titlesCnt != authorsCnt)
    {
        int a = 0;
    }

    commonCnt+=authorsCnt;

    //fileToWriteT->write(QString::number(commonCnt).toUtf8());
    //fileToWriteT->write(QByteArray("\n"));

    for(int i=0; i<authors.count();i++)
    {
        int commas = 0;
        QString author = authors[i];
        int len = author.length();
        while(len != 0)
        {
            int ind = author.indexOf(", ");
            if(ind != -1)
            {
                QString aaa = author.left(ind);
                fileToWriteA->write(aaa.toUtf8());
                fileToWriteA->write(QByteArray("\n"));
                fileToWriteT->write(titles[i].toUtf8());
                if(titles[i][titles[i].length()-1] != '\n')
                {
                    fileToWriteT->write(QByteArray("\n"));
                }

                fileToWriteS->write(subjects[i].toUtf8());
                fileToWriteS->write(QByteArray("\n"));

                author = author.mid(ind+3);
                len = author.length();
                commas++;
            }
            else
            {
                fileToWriteA->write(author.toUtf8());
                if(author[author.length()-1] != '\n')
                {
                    fileToWriteA->write(QByteArray("\n"));
                }
                fileToWriteT->write(titles[i].toUtf8());
                if(titles[i][titles[i].length()-1] != '\n')
                {
                    fileToWriteT->write(QByteArray("\n"));
                }

                fileToWriteS->write(subjects[i].toUtf8());
                fileToWriteS->write(QByteArray("\n"));

                len = 0;
            }
        }
    }

    fileToWriteT->flush();
    fileToWriteT->close();
    delete fileToWriteT;

    fileToWriteA->flush();
    fileToWriteA->close();
    delete fileToWriteA;

    fileToWriteS->flush();
    fileToWriteS->close();
    delete fileToWriteS;
*/
