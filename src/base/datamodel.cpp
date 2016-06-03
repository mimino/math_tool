#include <QDir>
#include <QPluginLoader>
#include <QtGlobal>
#include <QVector>
#include <QtMath>
#include <QDebug>

#include "plugin_interface.h"
#include "datamodel.h"
#include "errors.h"


DataModel::DataModel(QObject *parent) : QObject(parent)
{
	QDir dir;
	dir.cd(APPLICATION_PATH);
	dir.mkdir("data");
}

void DataModel::refresh(int subjectId)
{
    _curSubjectId = subjectId;
	int ret = refreshFromLocal();
	if(ret != ERROR_NONE)
	{
		refreshFromRemote();
	}
}

int DataModel::refreshFromLocal()
{
	int ret = refreshKeywords();
	if(ret == ERROR_NONE)
	{
		ret = refreshAuthors();
		if(ret == ERROR_NONE)
		{
			ret = refreshPublications();
		}
	}

	return ret;
}

int DataModel::refreshAuthors()
{
	QString authorsFN = QString("%1data/authors-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QString coauthorsFN = QString("%1data/coauthors-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QString keywordsFN = QString("%1data/authors-keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	if(!QFile::exists(authorsFN) || !QFile::exists(authorsFN) || !QFile::exists(keywordsFN))
	{
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	QFile authorsFile;
	authorsFile.setFileName(authorsFN);
	if (!authorsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	QList<QByteArray> strings;
	while (!authorsFile.atEnd()) {
		QByteArray line = authorsFile.readLine();
		strings += line.split('|');
	}

	for(int i=0; i<strings.count(); i+=2)
	{
		_authors.append(Author(strings[i], "", QString(strings[i+1]).toInt()));
	}

	authorsFile.close();


	QFile coauthorsFile;
	coauthorsFile.setFileName(coauthorsFN);
	if (!coauthorsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	int index = 0;
	while (!coauthorsFile.atEnd()) {
		strings.clear();
		QByteArray line = coauthorsFile.readLine();
		strings += line.split(' ');

		for(int i=0; i<strings.count(); i++)
		{
			if(!strings[i].isEmpty())
			{
				int id = QString(strings[i]).toInt();
				_authors[index]._coauthors.append(id);
			}
		}
		index++;
	}

	coauthorsFile.close();

	QFile keywordsFile;
	keywordsFile.setFileName(keywordsFN);
	if (!keywordsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	index = 0;
	while (!keywordsFile.atEnd()) {
		strings.clear();
		QByteArray line = keywordsFile.readLine();
		strings += line.split(' ');

		for(int i=0; i<strings.count(); i++)
		{
			if(!strings[i].isEmpty())
			{
				int id = QString(strings[i]).toInt();
				_authors[index]._keywords.append(id);
			}
		}
		index++;
	}

	keywordsFile.close();

	return ERROR_NONE;
}

int DataModel::refreshKeywords()
{
	QString keywordsFN = QString("%1data/keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QString keywordsRelationsFN = QString("%1data/keywords-relations-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	if(!QFile::exists(keywordsFN) || !QFile::exists(keywordsRelationsFN))
	{
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	QFile keywordsFile;
	keywordsFile.setFileName(keywordsFN);
	if (!keywordsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	QList<QByteArray> strings;
	while (!keywordsFile.atEnd()) {
		QByteArray line = keywordsFile.readLine();
		strings += line.split('|');
	}

	for(int i=0; i<strings.count(); i+=2)
	{
		_keywords.append(Keyword(strings[i], "", QString(strings[i+1]).toInt()));
	}

	keywordsFile.close();


	QFile keywordsRelationsFile;
	keywordsRelationsFile.setFileName(keywordsRelationsFN);
	if (!keywordsRelationsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return ERROR_LOCAL_STORAGE_BROKEN;
	}

	strings.clear();
	while (!keywordsRelationsFile.atEnd()) {
		QByteArray line = keywordsRelationsFile.readLine();
		strings += line.split(' ');
	}

	for(int i=0; i<strings.count(); i+=2)
	{
		int p0 = QString(strings[i]).toInt();
		int p1 = QString(strings[i+1]).toInt();
		QPair<int, int> pair(p0, p1);
		_keywordsRelations.append(pair);
	}

	keywordsRelationsFile.close();

	return ERROR_NONE;
}

int DataModel::refreshPublications()
{
	return ERROR_NONE;
}

void DataModel::storeAuthors()
{
	QString authorsFN = QString("%1data/authors-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile authorsFile;
	authorsFile.setFileName(authorsFN);
	if (!authorsFile.open(QIODevice::WriteOnly)) {
		return;
	}

	QByteArray data;
	for(int i=0; i<_authors.count(); i++)
	{
		QString line = QString("%1|%2\n").arg(_authors[i]._title).arg(_authors[i]._weight);
		data.append(line);
	}

	authorsFile.write(data);
	authorsFile.flush();
	authorsFile.close();

	QString coauthorsFN = QString("%1data/coauthors-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile coauthorsFile;
	coauthorsFile.setFileName(coauthorsFN);
	if (!coauthorsFile.open(QIODevice::WriteOnly)) {
		return;
	}

	data.clear();
	for(int i=0; i<_authors.count(); i++)
	{
		QString line;
		for(int k=0; k<_authors[i]._coauthors.count(); k++)
		{
			line.append(QString("%1 ").arg(_authors[i]._coauthors[k]));
		}

		line.append("\n");
		data.append(line);
	}

	coauthorsFile.write(data);
	coauthorsFile.flush();
	coauthorsFile.close();

	QString keywordsFN = QString("%1data/authors-keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile keywordsFile;
	keywordsFile.setFileName(keywordsFN);
	if (!keywordsFile.open(QIODevice::WriteOnly)) {
		return;
	}

	data.clear();
	for(int i=0; i<_authors.count(); i++)
	{
		QString line;
		for(int k=0; k<_authors[i]._keywords.count(); k++)
		{
			line.append(QString("%1 ").arg(_authors[i]._keywords[k]));
		}

		line.append("\n");
		data.append(line);
	}

	keywordsFile.write(data);
	keywordsFile.flush();
	keywordsFile.close();
}

void DataModel::storeKeywords()
{
	QString keywordsFN = QString("%1data/keywords-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile keywordsFile;
	keywordsFile.setFileName(keywordsFN);
	if (!keywordsFile.open(QIODevice::WriteOnly)) {
		return;
	}

	QByteArray data;
	for(int i=0; i<_keywords.count(); i++)
	{
		QString line = QString("%1|%2\n").arg(_keywords[i]._title).arg(_keywords[i]._weight);
		data.append(line);
	}

	keywordsFile.write(data);
	keywordsFile.flush();
	keywordsFile.close();


	QString keywordsRelationsFN = QString("%1data/keywords-relations-%2").arg(APPLICATION_PATH).arg(_curSubjectId);
	QFile keywordsRelationsFile;
	keywordsRelationsFile.setFileName(keywordsRelationsFN);
	if (!keywordsRelationsFile.open(QIODevice::WriteOnly)) {
		return;
	}

	data.clear();
	for(int i=0; i<_keywordsRelations.count(); i++)
	{
		QString line;
		line.append(QString("%1").arg(_keywordsRelations[i].first));
		line.append(" ");
		line.append(QString("%1").arg(_keywordsRelations[i].second));
		line.append("\n");

		data.append(line);
	}

	keywordsRelationsFile.write(data);
	keywordsRelationsFile.flush();
	keywordsRelationsFile.close();
}

void DataModel::storePublications()
{

}

void DataModel::refreshFromRemote()
{
    QDir pluginsDir(PLUGINS_PATH);
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            PluginInterface* pluginInterface = qobject_cast<PluginInterface*>(plugin);
            if(pluginInterface)
            {
                connect(dynamic_cast<QObject*>(pluginInterface), SIGNAL(pluginRefreshFinished(PluginInterface*)), SLOT(refreshFromRemoteFinished(PluginInterface*)));
                pluginInterface->refresh(_curSubjectId);
            }
        }
    }
}

void DataModel::refreshFromRemoteFinished(PluginInterface* plugin)
{
    plugin->getKeywords(_curSubjectId, _keywords, _keywordsRelations);
	plugin->getAuthors(_curSubjectId, _authors);

	storeToLocal();

	emit(refreshFinished());
}

void DataModel::storeToLocal()
{
	storeKeywords();
	storeAuthors();
	storePublications();
}

