#ifndef SPRITERPLUGIN_H
#define SPRITERPLUGIN_H

#include <QQmlExtensionPlugin>
#include "qtspritermodel.h"
#include "qtentityinstance.h"

class SpriterPlugin : public QQmlExtensionPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "be.wumbo.labsin.Spriter")

public:
	void registerTypes(const char *uri)
	{
		Q_ASSERT(uri == QLatin1String("Spriter"));
		qmlRegisterType<QtSpriterModel>(uri, 0, 1, "SpriterModel");
		qmlRegisterType<QtEntityInstance>(uri, 0, 1, "EntityInstance");
	}
};

#include "spriterplugin.moc"
#endif // SPRITERPLUGIN_H
