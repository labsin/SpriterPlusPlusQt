#include <QQuickView>
#include <QApplication>

#ifndef COMPILE_QML_PLUGIN
#include "qtspritermodel.h"
#include "qtentityinstance.h"
#endif

#include "qtspritermodel.h"

bool QtSpriterModel::threaded = true;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
#ifndef COMPILE_QML_PLUGIN
	qmlRegisterType<QtSpriterModel>("Spriter", 0, 1, "SpriterModel");
	qmlRegisterType<QtEntityInstance>("Spriter", 0, 1, "EntityInstance");
#endif
	QQuickView view;
//	QSurfaceFormat format = view.format();
//	format.setSamples(16);
//	view.setFormat(format);
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl::fromLocalFile("main.qml"));
	view.show();
	app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
