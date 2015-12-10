#include <QQuickView>
#include <QApplication>
#include "qsprintermodel.h"
#include "qentityinstance.h"
#include <QtQml/qqmldebug.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Using QQuickView
	qmlRegisterType<QSprinterModel>("QtSpriter", 1, 0, "SpriterModel");
	qmlRegisterType<QEntityInstance>("QtSpriter", 1, 0, "EntityInstance");
    QQuickView view;
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl::fromLocalFile("main.qml"));
	view.show();
	app.setQuitOnLastWindowClosed(true);
	//QQuickItem *object = view.rootObject();
    return app.exec();
}
