#include "qtjsonspriterfileattributewrapper.h"
#include "qtjsonspriterfileelementwrapper.h"

QtJSONSpriterFileAttributeWrapper::QtJSONSpriterFileAttributeWrapper(QtJSONSpriterFileElementWrapper *parent_object, QString name, QJsonValue value):
	m_value(value), m_name(name), m_parent_object(parent_object)
{
}


std::string QtJSONSpriterFileAttributeWrapper::getName()
{
	return m_name.toStdString();
}

bool QtJSONSpriterFileAttributeWrapper::isValid()
{
	return !m_value.isUndefined();
}

SpriterEngine::real QtJSONSpriterFileAttributeWrapper::getRealValue()
{
	return m_value.toDouble();
}

int QtJSONSpriterFileAttributeWrapper::getIntValue()
{
	return m_value.toInt();
}

std::string QtJSONSpriterFileAttributeWrapper::getStringValue()
{
	return m_value.toString().toStdString();
}

void QtJSONSpriterFileAttributeWrapper::advanceToNextAttribute()
{
	m_parent_object->advanceToNextAttribute(this);
}
