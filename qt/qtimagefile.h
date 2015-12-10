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
	QtImageFile(std::string initialFilePath, point initialDefaultPivot);
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

	QRect rect() const;

	QSGTexture* getTexture(QQuickWindow* window);

private:
	void initializeFile();
	void clearTextures();

	QImage m_image;
	QHash<QQuickWindow*,QSGTexture*> m_textureMap;
	QMutex m_textureMutex;
	int m_width;
	int m_height;
};
}

#endif // QTIMAGEFILE_H
