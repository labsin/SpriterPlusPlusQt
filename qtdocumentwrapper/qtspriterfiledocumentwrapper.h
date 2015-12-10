#ifndef QTXMLSPRITERFILEDOCUMENTWRAPPER_H
#define QTXMLSPRITERFILEDOCUMENTWRAPPER_H

#include "override/spriterfiledocumentwrapper.h"
#include <QDomDocument>

namespace SpriterEngine
{
	class QtXmlSpriterFileDocumentWrapper : public SpriterFileDocumentWrapper
	{
	public:
		QtXmlSpriterFileDocumentWrapper();

		void loadFile(std::string fileName) override;

	private:
		SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
		SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string & elementName) override;

		QDomDocument m_doc;
	};

}

#endif // QTXMLSPRITERFILEDOCUMENTWRAPPER_H
