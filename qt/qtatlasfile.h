#ifndef QTATLASFILE_H
#define QTATLASFILE_H

#include "override/atlasfile.h"

#include <QImage>
#include <QMutex>
#include <QHash>
#include <QRect>

class QSGTexture;
class QQuickWindow;
namespace SpriterEngine
{
class QtAtlasFile : public AtlasFile
{
public:
	QtAtlasFile(std::string initialFilePath);
	~QtAtlasFile();

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
	QMutex m_textureMapMutex;
	int m_width;
	int m_height;
};
}

#endif // QTATLASFILE_H
