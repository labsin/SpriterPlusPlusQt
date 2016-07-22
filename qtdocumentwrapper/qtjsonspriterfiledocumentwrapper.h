#ifndef QTJSONSPRITERFILEDOCUMENTWRAPPER_H
#define QTJSONSPRITERFILEDOCUMENTWRAPPER_H

#include "override/spriterfiledocumentwrapper.h"
#include <QJsonDocument>

class QtJSONSpriterFileDocumentWrapper : public SpriterEngine::SpriterFileDocumentWrapper
{
public:
	QtJSONSpriterFileDocumentWrapper();

	// SpriterFileDocumentWrapper interface
public:
	void loadFile(std::string fileName) override;

private:
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string &elementName) override;

	QJsonDocument m_doc;
};

#endif // QTJSONSPRITERFILEDOCUMENTWRAPPER_H
