#include "qsprintermodel.h"

#include "qtfilefactory.h"
#include "qtobjectfactory.h"

#include <spriterengine.h>
#include <global/settings.h>
#include <entity/entityinstance.h>

#include <QDebug>
#include <QTimerEvent>

QSprinterModel::QSprinterModel(QObject *parent):
	QObject(parent), m_model(nullptr)
{
	SpriterEngine::Settings::setErrorFunction(&error);
}

QSprinterModel::~QSprinterModel()
{
	if(m_model){
		delete m_model;
	}
}

void QSprinterModel::error(const std::string &errorMessage)
{
	qCritical() << QString::fromStdString(errorMessage);
}

SpriterEngine::EntityInstance* QSprinterModel::getNewEntityInstance(QString name)
{
	Q_ASSERT(m_model);
	return m_model->getNewEntityInstance(name.toStdString());
}

void QSprinterModel::setFile(QString file)
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
