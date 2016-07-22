#ifndef QTFILEFACTORY_H
#define QTFILEFACTORY_H

#include "spriterengine/override/filefactory.h"

namespace SpriterEngine
{
class QtFileFactory : public FileFactory
{
public:
    QtFileFactory();
	ImageFile *newImageFile(const std::string &initialFilePath, point initialDefaultPivot, atlasdata atlasData) override;
    SoundFile *newSoundFile(const std::string &initialFilePath) override;

    SpriterFileDocumentWrapper *newScmlDocumentWrapper();
};
}

#endif // QTFILEFACTORY_H
