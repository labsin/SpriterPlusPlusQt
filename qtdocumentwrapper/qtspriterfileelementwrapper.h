#ifndef QTXMLSPRITERFILEELEMENTWRAPPER_H
#define QTXMLSPRITERFILEELEMENTWRAPPER_H

#include "override/spriterfileelementwrapper.h"
#include <QDomElement>

namespace SpriterEngine
{
	class QtXmlSpriterFileElementWrapper : public SpriterFileElementWrapper
	{
	public:
		QtXmlSpriterFileElementWrapper(QDomElement initialElement);

		std::string getName() override;

		bool isValid() override;

		void advanceToNextSiblingElement() override;
		void advanceToNextSiblingElementOfSameName() override;

	private:
		SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute() override;
		SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute(const std::string & attributeName) override;

		SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
		SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string & elementName) override;

		SpriterFileElementWrapper *newElementWrapperFromNextSiblingElement() override;

		SpriterFileElementWrapper *newElementClone() override;

		QDomElement m_element;
	};

}

#endif // QTXMLSPRITERFILEELEMENTWRAPPER_H
