#ifndef QTXMLSPRITERFILEELEMENTWRAPPER_H
#define QTXMLSPRITERFILEELEMENTWRAPPER_H

#include "override/spriterfileelementwrapper.h"
#include <QDomElement>

class QtXmlSpriterFileElementWrapper : public SpriterEngine::SpriterFileElementWrapper
{
public:
	QtXmlSpriterFileElementWrapper(QDomElement initialElement);

	std::string getName() override;

	bool isValid() override;

	void advanceToNextSiblingElement() override;
	void advanceToNextSiblingElementOfSameName() override;

private:
	SpriterEngine::SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute() override;
	SpriterEngine::SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute(const std::string & attributeName) override;

	SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
	SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string & elementName) override;

	SpriterFileElementWrapper *newElementWrapperFromNextSiblingElement() override;

	SpriterFileElementWrapper *newElementClone() override;

	QDomElement m_element;
};

#endif // QTXMLSPRITERFILEELEMENTWRAPPER_H
