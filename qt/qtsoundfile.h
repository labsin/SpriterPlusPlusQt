#ifndef QTSOUNDFILE_H
#define QTSOUNDFILE_H

#include "override/soundfile.h"
#include <QSoundEffect>

namespace SpriterEngine {
class QtSoundFile : public SoundFile
{
public:
	QtSoundFile(std::string initialFilePath);

	SoundObjectInfoReference * newSoundInfoReference();

private:
};
}

#endif // QTSOUNDFILE_H
