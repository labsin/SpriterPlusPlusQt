#include "qtfilefactory.h"
#include "qtxmlspriterfiledocumentwrapper.h"
#include "qtimagefile.h"
#include "qtsoundfile.h"

using namespace SpriterEngine;

QtFileFactory::QtFileFactory()
{

}

ImageFile *QtFileFactory::newImageFile(const std::string &initialFilePath, point initialDefaultPivot)
{
	return new QtImageFile(initialFilePath, initialDefaultPivot);
}

SoundFile *QtFileFactory::newSoundFile(const std::string &initialFilePath)
{
	return new QtSoundFile(initialFilePath);
}

SpriterFileDocumentWrapper *QtFileFactory::newScmlDocumentWrapper()
{
	return new QtXmlSpriterFileDocumentWrapper();
}
