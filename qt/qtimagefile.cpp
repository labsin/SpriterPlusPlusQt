#include "qtimagefile.h"
#include "global/settings.h"

using namespace SpriterEngine;

SpriterEngine::QtImageFile::QtImageFile(std::string initialFilePath, point initialDefaultPivot):
	ImageFile(initialFilePath,initialDefaultPivot)
{
	initializeFile();
}


void SpriterEngine::QtImageFile::renderSprite(UniversalObjectInterface *spriteInfo)
{
	//Void
}

void SpriterEngine::QtImageFile::initializeFile()
{
	m_image.load(QString::fromStdString(path()));
	if(m_image.isNull()) {
		Settings::Settings::error("QtImageFile::initializeFile - pixmap unable to load file from path \"" + path() + "\"");
	}
}
