#include "qtsoundobjectinforeference.h"
#include "global/settings.h"

using namespace SpriterEngine;

QtSoundObjectInfoReference::QtSoundObjectInfoReference(QUrl url)
{
	m_soundEffect.setSource(url);
	if(m_soundEffect.status()!=QSoundEffect::Ready) {
			Settings::error("QtSoundFile::initializeFile - QSoundEffect unable to load file from path \"" + url.toString().toStdString() + "\"");
	}
}

void QtSoundObjectInfoReference::playTrigger()
{
	if (getTriggerCount())
	{
		m_soundEffect.play();
	}
}

void QtSoundObjectInfoReference::setPlaybackVolume()
{
	m_soundEffect.setVolume(getVolume());
}

void QtSoundObjectInfoReference::setPlaybackPanning()
{
	//Not possible with QSoundEffect
}
