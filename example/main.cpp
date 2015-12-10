#include <QQuickView>
#include <QApplication>
#include "qsprintermodel.h"
#include "qentityinstance.h"
#include <QtQml/qqmldebug.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	qmlRegisterType<QSprinterModel>("Spriter", 0, 1, "SpriterModel");
	qmlRegisterType<QEntityInstance>("Spriter", 0, 1, "EntityInstance");
    QQuickView view;
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl::fromLocalFile("main.qml"));
	view.show();
	app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
