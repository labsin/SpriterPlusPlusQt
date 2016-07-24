#include "qtspritermodel.h"

#include "qtfilefactory.h"
#include "qtobjectfactory.h"

#include <spriterengine.h>
#include <global/settings.h>
#include <entity/entityinstance.h>

#include <QDebug>
#include <QTimerEvent>

QtSpriterModel::QtSpriterModel(QObject *parent):
	QObject(parent), m_model(nullptr)
{
}

QtSpriterModel::~QtSpriterModel()
{
	if(m_model){
		delete m_model;
	}
}

void QtSpriterModel::error(const std::string &errorMessage)
{
	qCritical() << QString::fromStdString(errorMessage);
}

SpriterEngine::EntityInstance* QtSpriterModel::getNewEntityInstance(QString name)
{
	if(!m_model) {
		return nullptr;
	}
	return m_model->getNewEntityInstance(name.toStdString());
}

bool QtSpriterModel::renderBones() const
{
	return SpriterEngine::Settings::renderDebugBones;
}

bool QtSpriterModel::renderPoints() const
{
	return SpriterEngine::Settings::renderDebugPoints;
}

bool QtSpriterModel::renderBoxes() const
{
	return SpriterEngine::Settings::renderDebugBoxes;
}

void QtSpriterModel::setFile(QString file)
{
	if (m_file == file)
		return;

	if(m_model) {
		delete m_model;
		m_model = nullptr;
    }
    m_file = file;
	m_model = new SpriterEngine::SpriterModel(m_file.toStdString(), new SpriterEngine::QtFileFactory(), new SpriterEngine::QtObjectFactory());
	emit fileChanged(file);
}

void QtSpriterModel::setDebug(bool debug)
{
	if (m_debug == debug)
		return;

	m_debug = debug;
	if(m_debug) {
		SpriterEngine::Settings::setErrorFunction(&error);
		SpriterEngine::Settings::enableDebugBones = true;
	}
	else {
		SpriterEngine::Settings::setErrorFunction(SpriterEngine::Settings::nullError);
		SpriterEngine::Settings::enableDebugBones = false;
	}
	emit debugChanged(debug);
}

void QtSpriterModel::setRenderBones(bool renderBones)
{
	if (SpriterEngine::Settings::renderDebugBones == renderBones)
		return;

	SpriterEngine::Settings::renderDebugBones = renderBones;
	emit renderBonesChanged(renderBones);
}

void QtSpriterModel::setRenderPoints(bool renderPoints)
{
	if (SpriterEngine::Settings::renderDebugPoints == renderPoints)
		return;

	SpriterEngine::Settings::renderDebugPoints = renderPoints;
	emit renderPointsChanged(renderPoints);
}

void QtSpriterModel::setRenderBoxes(bool renderBoxes)
{
	if (SpriterEngine::Settings::renderDebugBoxes == renderBoxes)
		return;

	SpriterEngine::Settings::renderDebugBoxes = renderBoxes;
	emit renderBoxesChanged(renderBoxes);
}
