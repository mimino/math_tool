#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("testdb.db");
    if(!QFile::exists("testdb.db")) // Database does not exist yet
    {
        if (!db.open())
        {
            QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                  qApp->tr("Unable to establish a database connection.\n"
                                           "This example needs SQLite support. Please read "
                                           "the Qt SQL driver documentation for information how "
                                           "to build it.\n\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);
            return false;
        }

        QSqlQuery query;

        query.exec("create table authors (id int primary key, "
                                         "author varchar(40), "
                                         "organization varchar(40))");

        query.exec("insert into authors values(0, '<all>', '')");
        query.exec("insert into authors values(1, 'Ane Brun', 'NN')");
        query.exec("insert into authors values(2, 'Thomas Dybdahl', 'Dz')");
        query.exec("insert into authors values(3, 'Kaizers Orchestra', 'NY')");

        query.exec("create table publications (id int primary key, "
                                         "title varchar(50), "
                                         "year int, "
                                         "url varchar(150), "
                                         "journalid int)");

        query.exec("insert into publications values(0, 'Spending Time With Morgan', 2003, 1)");
        query.exec("insert into publications values(1, 'A Temporary Dive', 2005, 2)");
        query.exec("insert into publications values(2, '...The Great October Sound', 2002, 2)");
        query.exec("insert into publications values(3, 'Stray Dogs', 2003, 0)");
        query.exec("insert into publications values(4, 'One day you`ll dance for me, New York City', 2004, 1)");
        query.exec("insert into publications values(5, 'Ompa Til Du D\xf8r', 2001, 2)");
        query.exec("insert into publications values(6, 'Evig Pint', 2002, 0)");
        query.exec("insert into publications values(7, 'Maestro', 2005, 0)");


        query.exec("create table subjects (id int primary key, "
                                         "subject varchar(40))");

        query.exec("insert into subjects values(0, 'Mathematics')");
        query.exec("insert into subjects values(1, 'Algebra')");
        query.exec("insert into subjects values(2, 'Control & Optimization')");
        query.exec("insert into subjects values(3, 'Discrete Mathematics')");
        query.exec("insert into subjects values(4, 'Geometry')");
        query.exec("insert into subjects values(5, 'Group Theory')");
        query.exec("insert into subjects values(6, 'Mathematical Analysis')");
        query.exec("insert into subjects values(7, 'Mathematical Modelling')");
        query.exec("insert into subjects values(8, 'Mathematics Education')");
        query.exec("insert into subjects values(9, 'Number Theory')");
        query.exec("insert into subjects values(10, 'Probability')");
        query.exec("insert into subjects values(11, 'Statistics')");
        query.exec("insert into subjects values(12, 'Topology')");


        query.exec("create table journals (id int primary key, "
                                         "journal varchar(40))");

        query.exec("insert into journals values(0, 'UMN')");
        query.exec("insert into journals values(1, 'Izvestia')");
        query.exec("insert into journals values(2, 'Vestnik NNSU')");


        query.exec("create table publicationsubject (id int primary key, "
                                         "publicationid int, "
                                         "subjectid int)");

        query.exec("insert into publicationsubject values(0, 0, 1)");
        query.exec("insert into publicationsubject values(1, 1, 2)");
        query.exec("insert into publicationsubject values(2, 2, 5)");
        query.exec("insert into publicationsubject values(3, 3, 3)");
        query.exec("insert into publicationsubject values(4, 4, 5)");
        query.exec("insert into publicationsubject values(5, 5, 10)");


        query.exec("create table authorpublication (id int primary key, "
                                         "authorid int, "
                                         "publicationid int)");

        query.exec("insert into authorpublication values(0, 1, 1)");
        query.exec("insert into authorpublication values(1, 2, 2)");
        query.exec("insert into authorpublication values(2, 3, 5)");
        query.exec("insert into authorpublication values(3, 1, 3)");
        query.exec("insert into authorpublication values(4, 2, 6)");
        query.exec("insert into authorpublication values(5, 3, 7)");
        query.exec("insert into authorpublication values(6, 1, 4)");
    }
    else // Database exists. Just open it
    {
        if (!db.open()) {
            QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                  qApp->tr("Unable to establish a database connection.\n"
                                           "This example needs SQLite support. Please read "
                                           "the Qt SQL driver documentation for information how "
                                           "to build it.\n\n"
                                           "Click Cancel to exit."), QMessageBox::Cancel);
            return false;
        }
    }

    return true;
}

#endif // __DATABASE_H__
