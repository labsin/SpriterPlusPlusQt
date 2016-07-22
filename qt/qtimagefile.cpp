#include "qtimagefile.h"
#include "qtatlasfile.h"

#include "global/settings.h"
#include "override/atlasfile.h"

#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGTexture>
#include <QMutexLocker>

using namespace SpriterEngine;

QtImageFile::QtImageFile(std::string initialFilePath, point initialDefaultPivot, atlasdata atlasData):
	ImageFile(initialFilePath,initialDefaultPivot), m_width(0), m_height(0), m_rotated(false)
{
	initializeFile();
}

QtImageFile::~QtImageFile()
{
	clearTextures();
}

QSGTexture* QtImageFile::getTexture(QQuickWindow* window) {
	if(atlasFile) {
		QtAtlasFile* l_atlasFile = static_cast<SpriterEngine::QtAtlasFile*>(atlasFile);
		Q_ASSERT(l_atlasFile);
		return l_atlasFile->getTexture(window);
	}

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

QRect QtImageFile::targetRect() const
{
	if(!m_image.isNull())
		return m_image.rect();
	if(atlasFile) {
		if(m_rotated) {
			// Width and height is switched in framedata.
			return QRect(QPoint(atlasFrameData.spriteSourcePosition.y, atlasFrameData.spriteSourcePosition.x),
						QSize(atlasFrameData.spriteSourceSize.y, atlasFrameData.spriteSourceSize.x));
		}
		else {
			return QRect(QPoint(atlasFrameData.spriteSourcePosition.x, atlasFrameData.spriteSourcePosition.y),
						QSize(atlasFrameData.spriteSourceSize.x, atlasFrameData.spriteSourceSize.y));
		}
	}
	return QRect();
}

QRect QtImageFile::sourceRect() const
{
	if(!m_image.isNull())
		return m_image.rect();
	else if(atlasFile) {
		if(m_rotated) {
			// Width and height is switched in framedata.
			return QRect(QPoint(atlasFrameData.framePosition.x, atlasFrameData.framePosition.y),
						 QSize(atlasFrameData.frameSize.y, atlasFrameData.frameSize.x));
		}
		else {
			return QRect(QPoint(atlasFrameData.framePosition.x, atlasFrameData.framePosition.y),
						 QSize(atlasFrameData.frameSize.x, atlasFrameData.frameSize.y));
		}
	}
	return QRect();

}

bool QtImageFile::rotated()
{
	if(atlasFile) {
		return m_rotated;
	}
	return false;
}

void QtImageFile::initializeFile()
{
	m_image.load(QString::fromStdString(path()));
	if(m_image.isNull()) {
		Settings::Settings::error("QtImageFile::initializeFile - unable to load file from path \"" + path() + "\"");
		clearTextures();
		return;
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


void SpriterEngine::QtImageFile::setAtlasFile(AtlasFile *initialAtlasFile, atlasframedata initialAtlasFrameData)
{
	ImageFile::setAtlasFile(initialAtlasFile, initialAtlasFrameData);
	m_width = initialAtlasFrameData.sourceSize.x;
	m_height = initialAtlasFrameData.sourceSize.y;

	m_rotated = atlasFrameData.rotated;
}
