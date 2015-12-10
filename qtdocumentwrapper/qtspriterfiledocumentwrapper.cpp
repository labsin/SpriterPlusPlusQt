#include "qtspriterfiledocumentwrapper.h"

#include "qtspriterfileelementwrapper.h"
#include <QFile>
#include <QString>
#include <QDomElement>

using namespace SpriterEngine;
QtXmlSpriterFileDocumentWrapper::QtXmlSpriterFileDocumentWrapper() :
	m_doc("spriterxml")
{
}

void QtXmlSpriterFileDocumentWrapper::loadFile(std::string fileName)
{
	QFile file(QString::fromStdString(fileName));
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!m_doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();
}

SpriterFileElementWrapper * QtXmlSpriterFileDocumentWrapper::newElementWrapperFromFirstElement()
{
	return new QtXmlSpriterFileElementWrapper(m_doc.firstChildElement());
}

SpriterFileElementWrapper * QtXmlSpriterFileDocumentWrapper::newElementWrapperFromFirstElement(const std::string & elementName)
{
	return new QtXmlSpriterFileElementWrapper(m_doc.firstChildElement(QString::fromStdString(elementName)));
}
