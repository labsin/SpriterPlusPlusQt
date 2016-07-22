#ifndef QTJSONSPRITERFILEATTRIBUTEWRAPPER_H
#define QTJSONSPRITERFILEATTRIBUTEWRAPPER_H

#include "override/spriterfileattributewrapper.h"

#include <QJsonValue>
#include <QString>

class QtJSONSpriterFileElementWrapper;

class QtJSONSpriterFileAttributeWrapper : public SpriterEngine::SpriterFileAttributeWrapper
{
public:
	QtJSONSpriterFileAttributeWrapper(QtJSONSpriterFileElementWrapper* parent_object, QString name = QString(""), QJsonValue value = QJsonValue(QJsonValue::Undefined));

	// SpriterFileAttributeWrapper interface
public:
	std::string getName() override;
	bool isValid() override;
	SpriterEngine::real getRealValue() override;
	int getIntValue() override;
	std::string getStringValue() override;
	void advanceToNextAttribute() override;

private:
	QJsonValue m_value;
	QString m_name;
	QtJSONSpriterFileElementWrapper* m_parent_object;

	friend class QtJSONSpriterFileElementWrapper;
};

#endif // QTJSONSPRITERFILEATTRIBUTEWRAPPER_H
