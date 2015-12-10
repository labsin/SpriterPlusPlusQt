#include "qtspriterfileelementwrapper.h"

#include "qtspriterfileattributewrapper.h"
#include <QDomNamedNodeMap>
#include <QString>

using namespace SpriterEngine;

QtXmlSpriterFileElementWrapper::QtXmlSpriterFileElementWrapper(QDomElement initialElement):
	m_element(initialElement)
{
}

std::string QtXmlSpriterFileElementWrapper::getName()
{
	return m_element.tagName().toStdString();
}

bool QtXmlSpriterFileElementWrapper::isValid()
{
	return !m_element.isNull();
}

void QtXmlSpriterFileElementWrapper::advanceToNextSiblingElement()
{
	m_element = m_element.nextSiblingElement();
}

void QtXmlSpriterFileElementWrapper::advanceToNextSiblingElementOfSameName()
{
	m_element = m_element.nextSiblingElement(QString::fromStdString(getName()));
}

SpriterFileAttributeWrapper * QtXmlSpriterFileElementWrapper::newAttributeWrapperFromFirstAttribute()
{
	return new QtXmlSpriterFileAttributeWrapper(m_element.attributes());
}

SpriterFileAttributeWrapper * QtXmlSpriterFileElementWrapper::newAttributeWrapperFromFirstAttribute(const std::string & attributeName)
{
	return new QtXmlSpriterFileAttributeWrapper(m_element.attributeNode(QString::fromStdString(attributeName)));
}

SpriterFileElementWrapper * QtXmlSpriterFileElementWrapper::newElementWrapperFromFirstElement()
{
	return new QtXmlSpriterFileElementWrapper(m_element.firstChildElement());
}

SpriterFileElementWrapper * QtXmlSpriterFileElementWrapper::newElementWrapperFromFirstElement(const std::string & elementName)
{
	return new QtXmlSpriterFileElementWrapper(m_element.firstChildElement(QString::fromStdString(elementName)));
}

SpriterFileElementWrapper * QtXmlSpriterFileElementWrapper::newElementWrapperFromNextSiblingElement()
{
	return new QtXmlSpriterFileElementWrapper(m_element.nextSiblingElement(m_element.tagName()));
}

SpriterFileElementWrapper * QtXmlSpriterFileElementWrapper::newElementClone()
{
	return new QtXmlSpriterFileElementWrapper(m_element);
}
