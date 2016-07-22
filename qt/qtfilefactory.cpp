#include "qtfilefactory.h"
#include "qtxmlspriterfiledocumentwrapper.h"
#include "qtjsonspriterfiledocumentwrapper.h"
#include "qtimagefile.h"
#include "qtsoundfile.h"
#include "qtatlasfile.h"

using namespace SpriterEngine;

QtFileFactory::QtFileFactory()
{

}

ImageFile *QtFileFactory::newImageFile(const std::string &initialFilePath, point initialDefaultPivot, atlasdata atlasData)
{
	return new QtImageFile(initialFilePath, initialDefaultPivot, atlasData);
}

SoundFile *QtFileFactory::newSoundFile(const std::string &initialFilePath)
{
	return new QtSoundFile(initialFilePath);
}

AtlasFile *QtFileFactory::newAtlasFile(const std::string &initialFilePath)
{
	return new QtAtlasFile(initialFilePath);
}

SpriterFileDocumentWrapper *QtFileFactory::newScmlDocumentWrapper()
{
	return new QtXmlSpriterFileDocumentWrapper();
}

SpriterFileDocumentWrapper *QtFileFactory::newSconDocumentWrapper()
{
	return new QtJSONSpriterFileDocumentWrapper();
}
