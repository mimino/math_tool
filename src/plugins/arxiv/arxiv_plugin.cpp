
#include "arxiv_plugin.h"

ArxivPlugin::ArxivPlugin()
{
}

ArxivPlugin::~ArxivPlugin()
{
}

QString ArxivPlugin::name()
{
	QString name("Arxiv");
	return name;
}

int ArxivPlugin::refresh()
{
	return 0;
}

Q_EXPORT_PLUGIN2(arxiv-plugin, ArxivPlugin);
