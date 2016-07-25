#include "qtspritermodel.h"

#include "qtfilefactory.h"
#include "qtobjectfactory.h"
#include "qtentityinstance.h"

#include <spriterengine.h>
#include <global/settings.h>
#include <entity/entityinstance.h>

#include <QDebug>
#include <QTimerEvent>

class QtSpriterModelWorker : public QObject
{
	Q_OBJECT
public:
	QtSpriterModelWorker();
	~QtSpriterModelWorker();

	bool isLoaded();

signals:
	void loaded();
	void newEntityInstance(QString name, QtEntityInstance* instance, SpriterEngine::EntityInstance *entity);

public slots:
	void load(QString fileName);
	void getNewEntityInstance(QString name, QtEntityInstance* instance);

private:
	void setLoaded(bool isLoaded);

	SpriterEngine::SpriterModel *m_model;
	bool m_loaded;
	QMutex m_loadedMutex;
};

QtSpriterModelWorker::QtSpriterModelWorker():
	m_model(nullptr), m_loaded(false)
{
}

QtSpriterModelWorker::~QtSpriterModelWorker()
{
	delete m_model;
}

void QtSpriterModelWorker::getNewEntityInstance(QString name, QtEntityInstance* instance)
{
	if(!m_model) {
		connect(this, &QtSpriterModelWorker::loaded, this, [&]() {
			if(!m_model) {
				emit newEntityInstance(name, instance, nullptr);
			}
			emit newEntityInstance(name, instance, m_model->getNewEntityInstance(name.toStdString()));
		});
		emit newEntityInstance(name, instance, nullptr);
	}
	emit newEntityInstance(name, instance, m_model->getNewEntityInstance(name.toStdString()));
}

bool QtSpriterModelWorker::isLoaded()
{
	QMutexLocker locker(&m_loadedMutex);
	return m_loaded;
}

void QtSpriterModelWorker::load(QString fileName)
{
	if(m_model) {
		delete m_model;
	}
	setLoaded(false);
	m_model = new SpriterEngine::SpriterModel(fileName.toStdString(), new SpriterEngine::QtFileFactory(), new SpriterEngine::QtObjectFactory());
	setLoaded(true);
}

void QtSpriterModelWorker::setLoaded(bool isLoaded)
{
	QMutexLocker locker(&m_loadedMutex);
	m_loaded = isLoaded;
	if(isLoaded) {
		loaded();
	}
}

QtSpriterModel::QtSpriterModel(QObject *parent):
	QObject(parent), m_loaded(false)
{
	QtSpriterModelWorker *worker = new QtSpriterModelWorker();
	worker->moveToThread(&m_workerThread);
	connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
	connect(this, &QtSpriterModel::fileChanged, worker, &QtSpriterModelWorker::load);
	connect(this, &QtSpriterModel::getNewEntityInstance, worker, &QtSpriterModelWorker::getNewEntityInstance);
	connect(worker, &QtSpriterModelWorker::newEntityInstance, this, &QtSpriterModel::newEntityInstance);
	connect(worker, &QtSpriterModelWorker::loaded, this, &QtSpriterModel::setLoaded);
	m_workerThread.start();
}

QtSpriterModel::~QtSpriterModel()
{
	m_workerThread.quit();
	m_workerThread.wait();
}

void QtSpriterModel::error(const std::string &errorMessage)
{
	qCritical() << QString::fromStdString(errorMessage);
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

	m_file = file;
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

void QtSpriterModel::newEntityInstance(QString name, QtEntityInstance *instance, SpriterEngine::EntityInstance * entity)
{
	instance->setNewEntityInstance(name, entity);
}

#include "qtspritermodel.moc"
