#include "qtimagefile.h"
#include "global/settings.h"
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGTexture>
#include <QMutexLocker>

using namespace SpriterEngine;

QtImageFile::QtImageFile(std::string initialFilePath, point initialDefaultPivot):
	ImageFile(initialFilePath,initialDefaultPivot), m_width(0), m_height(0)
{
	initializeFile();
}

QtImageFile::~QtImageFile()
{
	clearTextures();
}

QSGTexture* QtImageFile::getTexture(QQuickWindow* window) {
	QMutexLocker locker(&m_textureMapMutex);
	QSGTexture* texture = m_textureMap.value(window);
	if(texture)
		return texture;
	if(m_image.isNull())
		return nullptr;
	texture = window->createTextureFromImage(m_image, QQuickWindow::TextureCanUseAtlas);
	m_textureMap.insert(window,texture);
	return texture;
}


void QtImageFile::renderSprite(UniversalObjectInterface *spriteInfo)
{
	//Void
}

QRect QtImageFile::rect() const
{
	return m_image.rect();
}

void QtImageFile::initializeFile()
{
	m_image.load(QString::fromStdString(path()));
	if(m_image.isNull()) {
		Settings::Settings::error("QtImageFile::initializeFile - pixmap unable to load file from path \"" + path() + "\"");
	}
	m_width = m_image.width();
	m_height = m_image.height();
	clearTextures();
}

void QtImageFile::clearTextures()
{
	QMutexLocker locker(&m_textureMapMutex);
	for(QHash<QQuickWindow*,QSGTexture*>::iterator it = m_textureMap.begin(), end = m_textureMap.end(); it != end; it++) {
		delete it.value();
	}
	m_textureMap.clear();
}
