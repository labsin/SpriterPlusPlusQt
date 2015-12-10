#ifndef QTIMAGEFILE_H
#define QTIMAGEFILE_H

#include "override/imagefile.h"
#include <QImage>

namespace SpriterEngine
{
class QtImageFile : public ImageFile
{
public:
	QtImageFile(std::string initialFilePath, point initialDefaultPivot);

	void renderSprite(UniversalObjectInterface *spriteInfo) override;
	const QImage & image() {
		return m_image;
	}

private:
	void initializeFile();

	QImage m_image;
};
}

#endif // QTIMAGEFILE_H
