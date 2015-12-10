#include "qtspriterfileattributewrapper.h"
#include "global/settings.h"

using namespace SpriterEngine;

QtXmlSpriterFileAttributeWrapper::QtXmlSpriterFileAttributeWrapper(const QDomAttr attribute):
	m_attribute(attribute)
{
}

QtXmlSpriterFileAttributeWrapper::QtXmlSpriterFileAttributeWrapper(QDomNamedNodeMap noteMap, int index):
	m_noteMap(noteMap), m_attribute(m_noteMap.item(index).toAttr()), m_index(index)
{
}

std::string QtXmlSpriterFileAttributeWrapper::getName()
{
	return m_attribute.name().toStdString();
}

bool QtXmlSpriterFileAttributeWrapper::isValid()
{
	return !m_attribute.isNull();
}

real QtXmlSpriterFileAttributeWrapper::getRealValue()
{
	bool ok;
	real ret = m_attribute.value().toDouble(&ok);
	if(!ok) {
		Settings::error("QtXmlSpriterFileAttributeWrapper: "+getName()+" is not a double");
	}
	return ret;
}

int QtXmlSpriterFileAttributeWrapper::getIntValue()
{
	bool ok;
	int ret = m_attribute.value().toInt(&ok);
	if(!ok) {
		Settings::error("QtXmlSpriterFileAttributeWrapper: "+getName()+" is not an int");
	}
	return ret;
}

std::string QtXmlSpriterFileAttributeWrapper::getStringValue()
{
	return m_attribute.value().toStdString();
}

void QtXmlSpriterFileAttributeWrapper::advanceToNextAttribute()
{
	Q_ASSERT(!m_noteMap.isEmpty());
	m_index++;
	m_attribute = m_noteMap.item(m_index).toAttr();
}
