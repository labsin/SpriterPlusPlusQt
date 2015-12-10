#include "qtsoundfile.h"
#include "qtsoundobjectinforeference.h"

using namespace SpriterEngine;

QtSoundFile::QtSoundFile(std::string initialFilePath):
	SoundFile(initialFilePath)
{
}

SoundObjectInfoReference *QtSoundFile::newSoundInfoReference()
{
	return new QtSoundObjectInfoReference(QUrl::fromLocalFile(QString::fromStdString(path())));
}
