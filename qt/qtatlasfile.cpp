#include "qtatlasfile.h"
#include "global/settings.h"
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGTexture>
#include <QMutexLocker>

using namespace SpriterEngine;

QtAtlasFile::QtAtlasFile(std::string initialFilePath):
	AtlasFile(initialFilePath), m_width(0), m_height(0)
{
	initializeFile();
}

QtAtlasFile::~QtAtlasFile()
{
	clearTextures();
}

QSGTexture* QtAtlasFile::getTexture(QQuickWindow* window) {
	QMutexLocker locker(&m_textureMapMutex);
	QSGTexture* texture = m_textureMap.value(window);
	if(texture)
		return texture;
	if(m_image.isNull())
		return nullptr;
	texture = window->createTextureFromImage(m_image);
	m_textureMap.insert(window,texture);
	return texture;
}

QRect QtAtlasFile::rect() const
{
	return m_image.rect();
}

void QtAtlasFile::initializeFile()
{
	m_image.load(QString::fromStdString(path()));
	if(m_image.isNull()) {
		Settings::Settings::error("QtAtlasFile::initializeFile - pixmap unable to load file from path \"" + path() + "\"");
	}
	m_width = m_image.width();
	m_height = m_image.height();
	clearTextures();
}

void QtAtlasFile::clearTextures()
{
	QMutexLocker locker(&m_textureMapMutex);
	for(QHash<QQuickWindow*,QSGTexture*>::iterator it = m_textureMap.begin(), end = m_textureMap.end(); it != end; it++) {
		delete it.value();
	}
	m_textureMap.clear();
}
