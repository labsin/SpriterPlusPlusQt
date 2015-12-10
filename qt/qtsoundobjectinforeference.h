#ifndef QTSOUNDOBJECTINFOREFERENCE_H
#define QTSOUNDOBJECTINFOREFERENCE_H

#include "override/soundobjectinforeference.h"
#include <QSoundEffect>

namespace SpriterEngine {
class QtSoundObjectInfoReference : public SoundObjectInfoReference
{
public:
        QtSoundObjectInfoReference(QUrl url);

        void playTrigger() override;

private:
        void setPlaybackVolume() override;
        void setPlaybackPanning() override;

		QSoundEffect m_soundEffect;
};
}

#endif // QTSOUNDOBJECTINFOREFERENCE_H
