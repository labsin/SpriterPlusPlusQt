#ifndef QTXMLSPRITERFILEDOCUMENTWRAPPER_H
#define QTXMLSPRITERFILEDOCUMENTWRAPPER_H

#include "override/spriterfiledocumentwrapper.h"
#include <QDomDocument>

class QtXmlSpriterFileDocumentWrapper : public SpriterEngine::SpriterFileDocumentWrapper
{
public:
	QtXmlSpriterFileDocumentWrapper();

	void loadFile(std::string fileName) override;

private:
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string & elementName) override;

	QDomDocument m_doc;
};

#endif // QTXMLSPRITERFILEDOCUMENTWRAPPER_H
