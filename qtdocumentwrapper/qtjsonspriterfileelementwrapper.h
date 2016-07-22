#ifndef QTJSONSPRITERFILEELEMENTWRAPPER_H
#define QTJSONSPRITERFILEELEMENTWRAPPER_H

#include "override/spriterfileelementwrapper.h"
#include <QJsonArray>

class QtJSONSpriterFileAttributeWrapper;

class QtJSONSpriterFileElementWrapper : public SpriterEngine::SpriterFileElementWrapper
{
public:
	QtJSONSpriterFileElementWrapper(QtJSONSpriterFileElementWrapper* m_parent=nullptr, QString name = QString(""), QJsonArray array = QJsonArray(), int index = 0);

	// SpriterFileElementWrapper interface
public:
	std::string getName() override;
	bool isValid() override;
	void advanceToNextSiblingElement() override;
	void advanceToNextAttribute(QtJSONSpriterFileAttributeWrapper* from_attribute);
	void advanceToNextSiblingElementOfSameName() override;

private:
	SpriterEngine::SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute() override;
	SpriterEngine::SpriterFileAttributeWrapper *newAttributeWrapperFromFirstAttribute(const std::string &attributeName) override;
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement() override;
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromFirstElement(const std::string &elementName) override;
	SpriterEngine::SpriterFileElementWrapper *newElementWrapperFromNextSiblingElement() override;
	SpriterEngine::SpriterFileElementWrapper *newElementClone() override;

	QJsonArray m_array;
	int m_index;
	QString m_name;

	QtJSONSpriterFileElementWrapper *m_parent;
};

#endif // QTJSONSPRITERFILEELEMENTWRAPPER_H
