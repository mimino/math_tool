#include <QtGui>
#include <QtSql>

#include "authorswidget.h"
#include "articleswidget.h"
#include "contentwidget.h"
#include "journalswidget.h"
#include "organizationswidget.h"


ContentWidget::ContentWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setupTabs();
}

void ContentWidget::setupTabs()
{
    QStringList groups;
    groups << "Authors" << "Publications" << "Journals" << "Organizations" << "Keywords";

    AuthorsWidget* widget = new AuthorsWidget();
    this->addTab(widget, "Authors");

    ArticlesWidget* articles = new ArticlesWidget();
    this->addTab(articles, "Articles");

    //JournalsWidget* journals = new JournalsWidget(model);
    //this->addTab(journals, "Journals");

    //OrganizationsWidget* organizations = new OrganizationsWidget(model);
    //this->addTab(organizations, "Organizations");
}
