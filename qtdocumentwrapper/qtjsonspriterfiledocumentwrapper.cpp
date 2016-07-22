#include "qtjsonspriterfiledocumentwrapper.h"

#include "qtjsonspriterfileelementwrapper.h"
#include "global/settings.h"
#include <QFile>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

QtJSONSpriterFileDocumentWrapper::QtJSONSpriterFileDocumentWrapper()
{
}


void QtJSONSpriterFileDocumentWrapper::loadFile(std::string fileName)
{
	QFile file(QString::fromStdString(fileName));
	if (!file.open(QIODevice::ReadOnly)) {
		SpriterEngine::Settings::error("QtJSONSpriterFileDocumentWrapper::loadFile - Can't open file: " + fileName);
		return;
	}
	m_doc = QJsonDocument::fromJson(file.readAll());
	file.close();
}

SpriterEngine::SpriterFileElementWrapper *QtJSONSpriterFileDocumentWrapper::newElementWrapperFromFirstElement()
{
	if(m_doc.isObject()) {
		QJsonArray arr = QJsonArray();
		arr.append(QJsonValue(m_doc.object()));
		return new QtJSONSpriterFileElementWrapper(nullptr, "spriter_data", arr, 0);
	}
	SpriterEngine::Settings::error("JSON document doesn't contain an object");
	return new QtJSONSpriterFileElementWrapper();
}

SpriterEngine::SpriterFileElementWrapper *QtJSONSpriterFileDocumentWrapper::newElementWrapperFromFirstElement(const std::string &elementName)
{
	// Don't know what else to do here
	Q_ASSERT(elementName.compare("spriter_data") == 0);
	return newElementWrapperFromFirstElement();
}
