#ifndef QTXMLSPRITERFILEATTRIBUTEWRAPPER_H
#define QTXMLSPRITERFILEATTRIBUTEWRAPPER_H

#include "override/spriterfileattributewrapper.h"
#include <QDomAttr>

class QtXmlSpriterFileAttributeWrapper : public SpriterEngine::SpriterFileAttributeWrapper
{
public:
	QtXmlSpriterFileAttributeWrapper(QDomAttr attribute);
	QtXmlSpriterFileAttributeWrapper(QDomNamedNodeMap noteMap, int index=0);

	std::string getName() override;

	bool isValid() override;

	SpriterEngine::real getRealValue() override;
	int getIntValue() override;
	std::string getStringValue() override;

	void advanceToNextAttribute() override;

private:
	QDomNamedNodeMap m_noteMap;
	QDomAttr m_attribute;
	int m_index;
};

#endif // QTXMLSPRITERFILEATTRIBUTEWRAPPER_H
