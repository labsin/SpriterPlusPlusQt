#include "qtjsonspriterfileelementwrapper.h"

#include "qtjsonspriterfileattributewrapper.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QStringList>
#include <QRegularExpression>

using namespace SpriterEngine;

QtJSONSpriterFileElementWrapper::QtJSONSpriterFileElementWrapper(QtJSONSpriterFileElementWrapper *parent, QString name, QJsonArray array, int index) :
	m_array(array), m_index(index), m_name(name), m_parent(parent)
{
}

std::string QtJSONSpriterFileElementWrapper::getName()
{
	return m_name.toStdString();
}

bool QtJSONSpriterFileElementWrapper::isValid()
{
	//Also works for empty array and m_index=0
	if(m_index >= m_array.count())
		return false;
	QJsonValue object_value = m_array.at(m_index);
	if(! object_value.isObject()) {
		return false;
	}
	return true;
}

void QtJSONSpriterFileElementWrapper::advanceToNextSiblingElement()
{
	// First try in the array
	m_index++;
	if(isValid()) {
		return;
	}

	// Then see for the next key in parent

	if(!m_parent) {
		return;
	}

	if(! m_parent->isValid())  {
		return;
	}

	QJsonValue object_value = m_parent->m_array.at(m_parent->m_index);
	QJsonObject object = object_value.toObject();
	QStringList keys = object.keys();
	int index = keys.indexOf(m_name);
	if(index < 0 ) {
		return;
	}
	for(int iii=index+1; iii<keys.length(); ++iii) {
		QString key = keys[iii];
		QJsonValue child_value = object.value(key);
		if(!(child_value.isObject() || child_value.isArray())) {
			continue;
		}
		if(child_value.isObject()) {
			QJsonArray arr = QJsonArray();
			arr.append(QJsonValue(child_value.toObject()));
			m_array = arr;
			m_index = 0;
			m_name = key;
		}
		else if(child_value.isArray()) {
			m_array = child_value.toArray();
			m_index = 0;
			m_name = key;
		}
		return;
	}
}

void QtJSONSpriterFileElementWrapper::advanceToNextAttribute(QtJSONSpriterFileAttributeWrapper *from_attribute)
{
	if(! isValid()) {
		from_attribute->m_value = QJsonValue(QJsonValue::Undefined);
		from_attribute->m_name = "";
		return;
	}
	if(! from_attribute->isValid()) {
		return;
	}
	QString from_name = QString::fromStdString(from_attribute->getName());
	QJsonValue object_value = m_array.at(m_index);
	QJsonObject object = object_value.toObject();
	QStringList keys = object.keys();
	int index = keys.indexOf(from_name);
	if(index < 0 ) {
		from_attribute->m_value = QJsonValue(QJsonValue::Undefined);
		from_attribute->m_name = "";
		return;
	}
	for(int iii=index+1; iii<keys.length(); ++iii) {
		QString key = keys[iii];
		QJsonValue attr_value = object.value(key);
		if(attr_value.isObject() || attr_value.isArray()) {
			continue;
		}
		from_attribute->m_value = attr_value;
		from_attribute->m_name = key;
		return;
	}
	from_attribute->m_value = QJsonValue(QJsonValue::Undefined);
	from_attribute->m_name = "";
	return;

}

void QtJSONSpriterFileElementWrapper::advanceToNextSiblingElementOfSameName()
{
	m_index++;
}

SpriterFileAttributeWrapper *QtJSONSpriterFileElementWrapper::newAttributeWrapperFromFirstAttribute()
{
	if(! isValid()) {
		return new QtJSONSpriterFileAttributeWrapper(this);
	}
	QJsonValue object_value = m_array.at(m_index);
	QJsonObject object = object_value.toObject();
	QStringList keys = object.keys();
	if(keys.length() <= 0 ) {
		return new QtJSONSpriterFileAttributeWrapper(this);
	}
	for(QString key : keys) {
		QJsonValue attr_value = object.value(key);
		if(attr_value.isObject() || attr_value.isArray()) {
			continue;
		}
		return new QtJSONSpriterFileAttributeWrapper(this, key, attr_value);
	}
	return new QtJSONSpriterFileAttributeWrapper(this);
}

SpriterFileAttributeWrapper *QtJSONSpriterFileElementWrapper::newAttributeWrapperFromFirstAttribute(const std::string &attributeName)
{
	QString l_attributeName = QString::fromStdString(attributeName);
	if(! isValid()) {
		return new QtJSONSpriterFileAttributeWrapper(this, l_attributeName);
	}
	QJsonValue object_value = m_array.at(m_index);
	QJsonObject object = object_value.toObject();
	if(! object.contains(l_attributeName)) {
		return new QtJSONSpriterFileAttributeWrapper(this, l_attributeName);
	}
	QJsonValue attr_value = object.value(l_attributeName);
	if(attr_value.isObject() || attr_value.isArray()) {
		return new QtJSONSpriterFileAttributeWrapper(this, l_attributeName);
	}
	// Not an array or object
	return new QtJSONSpriterFileAttributeWrapper(this, l_attributeName, attr_value);
}

SpriterFileElementWrapper *QtJSONSpriterFileElementWrapper::newElementWrapperFromFirstElement()
{
	if(! isValid()) {
		return new QtJSONSpriterFileElementWrapper(this);
	}
	QJsonValue object_value = m_array.at(m_index);
	QJsonObject object = object_value.toObject();
	QStringList keys = object.keys();
	if(keys.length() <= 0 ) {
		return new QtJSONSpriterFileElementWrapper(this);
	}
	QString key = keys[0];
	//TODO: while loop
	for(QString key : keys) {
		QJsonValue child_value = object.value(key);
		if(!(child_value.isObject() || child_value.isArray())) {
			continue;
		}
		if(child_value.isObject()) {
			QJsonArray arr = QJsonArray();
			arr.append(QJsonValue(child_value.toObject()));
			return new QtJSONSpriterFileElementWrapper(this, key, arr, 0);
		}
		else if(child_value.isArray()) {
			return new QtJSONSpriterFileElementWrapper(this, key, child_value.toArray(), 0);
		}
	}
	// Not an array or object
	return new QtJSONSpriterFileElementWrapper(this);
}

SpriterFileElementWrapper *QtJSONSpriterFileElementWrapper::newElementWrapperFromFirstElement(const std::string &elementName)
{
	QString l_elementName = QString::fromStdString(elementName);
	if(! isValid()) {
		return new QtJSONSpriterFileElementWrapper(this, l_elementName);
	}
	QJsonValue object_value = m_array.at(m_index);
	QJsonObject object = object_value.toObject();
	if(! object.contains(l_elementName)) {
		return new QtJSONSpriterFileElementWrapper(this, l_elementName);
	}
	QJsonValue child_value = object.value(l_elementName);
	if(child_value.isObject()) {
		// Is an attribute, but returning anyway.
		QJsonArray arr = QJsonArray();
		arr.append(child_value);
		return new QtJSONSpriterFileElementWrapper(this, l_elementName, arr, 0);
	}
	else if(child_value.isArray()) {
		return new QtJSONSpriterFileElementWrapper(this, l_elementName, child_value.toArray(), 0);
	}
	// Not an array or object
	return new QtJSONSpriterFileElementWrapper(this, l_elementName);
}

SpriterFileElementWrapper *QtJSONSpriterFileElementWrapper::newElementWrapperFromNextSiblingElement()
{
	return new QtJSONSpriterFileElementWrapper(m_parent, m_name, m_array, m_index+1);
}

SpriterFileElementWrapper *QtJSONSpriterFileElementWrapper::newElementClone()
{
	return new QtJSONSpriterFileElementWrapper(m_parent, m_name, m_array, m_index);
}
