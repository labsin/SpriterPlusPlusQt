#ifndef QTIMAGEFILE_H
#define QTIMAGEFILE_H

#include "override/imagefile.h"

#include <QImage>
#include <QMutex>
#include <QHash>
#include <QRect>

class QSGTexture;
class QQuickWindow;
namespace SpriterEngine
{
class QtImageFile : public ImageFile
{
public:
	QtImageFile(std::string initialFilePath, point initialDefaultPivot, atlasdata atlasData);
	~QtImageFile();

	void renderSprite(UniversalObjectInterface *spriteInfo) override;
	const QImage & image() {
		return m_image;
	}
	int width() {
		return m_width;
	}
	int height() {
		return m_height;
	}

	QRect targetRect() const;

	QRect sourceRect() const;

	bool rotated();

	QSGTexture* getTexture(QQuickWindow* window);

private:
	void initializeFile();
	void clearTextures();
	void setAtlasFile(AtlasFile *initialAtlasFile, atlasframedata initialAtlasFrameData) override;

	QImage m_image;
	QHash<QQuickWindow*,QSGTexture*> m_textureMap;
	QMutex m_textureMapMutex;
	int m_width;
	int m_height;

	bool m_rotated;
};
}

#endif // QTIMAGEFILE_H
