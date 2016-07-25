#include "qtentityinstance.h"

#include "qtimagefile.h"
#include "qtboneinstanceinfo.h"
#include "qsgspriterbase.h"
#include "qsgspriterobjectnode.h"
#include "qsgspriterbonenode.h"

#include <entity/entityinstance.h>
#include <objectinfo/universalobjectinterface.h>
#include <objectinfo/spriteobjectinfo.h>
#include <global/settings.h>

#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QSGTransformNode>
#include <QSGOpacityNode>
#include <QMutexLocker>

#include <utility>
#include <math.h>

QtEntityInstanceWorker::QtEntityInstanceWorker(QtSpriterModel *model) :
	m_currentEntity(nullptr), m_zOrder(nullptr)
{
	m_modelWorker = model->m_worker;
		this->moveToThread(&model->m_workerThread);
}

QtEntityInstanceWorker::~QtEntityInstanceWorker()
{
	for(auto it = m_entityMap.begin(), end = m_entityMap.end(); it != end; it++) {
		delete it.value();
	}
}

void QtEntityInstanceWorker::startResume()
{
	if(m_currentEntity) {
		m_currentEntity->startResumePlayback();
		emit running(true);
	}
}

void QtEntityInstanceWorker::pause()
{
	if(m_currentEntity) {
		m_currentEntity->pausePlayback();
		emit running(false);
	}
}

void QtEntityInstanceWorker::setCurrentAnimation(const QString animation)
{
	if(m_currentEntity) {
		m_currentEntity->setCurrentAnimation(animation.toStdString());
	}
}

void QtEntityInstanceWorker::setScale(const QPointF scale)
{
	if(m_currentEntity) {
		m_currentEntity->setScale(SpriterEngine::point{scale.x(),scale.y()});
	}
}

void QtEntityInstanceWorker::setPlaybackSpeedRatio(float speed)
{
	if(m_currentEntity) {
		m_currentEntity->setPlaybackSpeedRatio(speed);
	}
}

void QtEntityInstanceWorker::load(QtEntityInstanceData data)
{
	if(!m_modelWorker) {
		return;
	}
	if(m_currentEntity) {
		unload();
	}
	if(data.entityName.isEmpty()) {
		emit error("Entity name set to null.");
		return;
	}

	SpriterEngine::EntityInstance *entity = m_entityMap.value(data.entityName);
	if(!entity) {
		entity = m_modelWorker->getNewEntityInstance(data.entityName);
	}
	if(entity) {
		m_currentEntity = entity;

		if(!data.animationName.isEmpty()) {
			m_currentEntity->setCurrentAnimation(data.animationName.toStdString());
		}
		m_currentEntity->setScale(SpriterEngine::point{data.scale.x(),data.scale.y()});
		m_currentEntity->setPlaybackSpeedRatio(data.playbackSpeed);

		m_time.start();
		emit loaded(true);
		emit running(true);
	}
	else {
		m_currentEntity = nullptr;
		emit error("Couldn't create entity with name: " + data.entityName);
	}
}

void QtEntityInstanceWorker::update()
{
	if(!m_currentEntity)
		return;

	using namespace SpriterEngine;
	m_currentEntity->setTimeElapsed(m_time.restart());

	ObjectInterfaceVector* zOrder = m_currentEntity->getZOrder();

	// std::vector compares sequencially
	bool zOrderChanged = true;
	if(zOrder == m_zOrder) {
		zOrderChanged = false;
	}
	else {
		m_zOrder = zOrder;
	}

	if(m_currentEntity->animationJustFinished()) {
		emit finished();
		emit running(false);
	}
	if(m_currentEntity->animationJustLooped()) {
		emit looped();
	}

	emit updated(zOrderChanged, m_zOrder);
}

void QtEntityInstanceWorker::unload()
{
	m_currentEntity = nullptr;
	emit loaded(false);
	emit running(false);
}

QtEntityInstance::QtEntityInstance(QQuickItem *parent):
	QQuickItem(parent),
	m_model(nullptr),
	m_zOrder(nullptr),
	m_zOrderChanged(false),
	m_loaded(false),
	m_running(false)
{
	setFlag(ItemHasContents);
	connect(this, &QtEntityInstance::windowChanged, this, &QtEntityInstance::updateQQuickWindow);
	updateQQuickWindow(window());
}

QtEntityInstance::~QtEntityInstance()
{
	for(auto it = m_nodeMap.begin(), end = m_nodeMap.end(); it != end; it++) {
		delete it.value();
	}
}

void QtEntityInstance::setName(QString name)
{
	if (data.entityName == name)
		return;

	if(!data.entityName.isEmpty()) {
		unload();
	}

	data.entityName = name;
	workerLoad(data);
	emit nameChanged(name);
}

void QtEntityInstance::setModel(QtSpriterModel *model)
{
	if (m_model == model)
		return;

	m_model = model;

	emit workerDelete();

	if(m_model) {
		QtEntityInstanceWorker* worker = new QtEntityInstanceWorker(m_model);

		connect(this, &QtEntityInstance::workerLoad, worker, &QtEntityInstanceWorker::load);
		connect(this, &QtEntityInstance::workerSetCurrentAnimation, worker, &QtEntityInstanceWorker::setCurrentAnimation);
		connect(this, &QtEntityInstance::workerSetPlaybackSpeedRatio, worker, &QtEntityInstanceWorker::setPlaybackSpeedRatio);
		connect(this, &QtEntityInstance::workerSetScale, worker, &QtEntityInstanceWorker::setScale);
		connect(this, &QtEntityInstance::workerUpdate, worker, &QtEntityInstanceWorker::update);
		connect(this, &QtEntityInstance::workerDelete, worker, &QtEntityInstanceWorker::deleteLater);
		connect(this, &QtEntityInstance::destroyed, worker, &QtEntityInstanceWorker::deleteLater);
		connect(this, &QtEntityInstance::pause, worker, &QtEntityInstanceWorker::pause);
		connect(this, &QtEntityInstance::startResume, worker, &QtEntityInstanceWorker::startResume);
		connect(worker, &QtEntityInstanceWorker::error, this, &QtEntityInstance::setErrorString);
		connect(worker, &QtEntityInstanceWorker::looped, this, &QtEntityInstance::looped);
		connect(worker, &QtEntityInstanceWorker::finished, this, &QtEntityInstance::finished);
		connect(worker, &QtEntityInstanceWorker::loaded, this, &QtEntityInstance::setLoaded);
		connect(worker, &QtEntityInstanceWorker::updated, this, &QtEntityInstance::setZOrder);
		connect(worker, &QtEntityInstanceWorker::running, this, &QtEntityInstance::setRunning);


		if(!data.entityName.isEmpty()) {
			workerLoad(data);
		}
	}
	emit modelChanged(model);
}

void QtEntityInstance::updateQQuickWindow(QQuickWindow *window)
{
	if(window) {
		connect(window, &QQuickWindow::frameSwapped, this, &QtEntityInstance::workerUpdate);
		emit workerUpdate();
	}
}

void QtEntityInstance::setAnimation(QString animation)
{
	if (data.animationName == animation)
		return;

	data.animationName = animation;
	if(m_loaded) {
		emit workerSetCurrentAnimation(animation);
	}
	emit animationChanged(animation);
}

void QtEntityInstance::setScale(QPointF scale)
{
	if (data.scale == scale)
		return;

	data.scale = scale;
	if(m_loaded) {
		emit workerSetScale(scale);
	}
	emit scaleChanged(scale);
}

void QtEntityInstance::setSpeedRatio(float speedRatio)
{
	if (data.playbackSpeed == speedRatio)
		return;

	data.playbackSpeed = speedRatio;
	if(m_loaded) {
		emit workerSetPlaybackSpeedRatio(data.playbackSpeed);
	}
	emit speedRatioChanged(speedRatio);
}

void QtEntityInstance::setErrorString(QString error)
{
	if (m_errorString == error)
		return;

	m_errorString = error;
	emit errorStringChanged(error);
}

void QtEntityInstance::setLoaded(bool loaded) {
	if(loaded == m_loaded) {
		return;
	}

	m_loaded = loaded;
	emit loadedChanged(loaded);
}

void QtEntityInstance::setZOrder(bool zOrderChanged, SpriterEngine::ObjectInterfaceVector *zOrder) {
	QMutexLocker locker(&m_zOrderMutex);
	m_zOrder = zOrder;
	m_zOrderChanged = zOrderChanged;

	update();
}

QSGSpriterBase *QtEntityInstance::getQSGSpriterNode(SpriterEngine::UniversalObjectInterface *interface)
{
	QSGSpriterBase* node = m_nodeMap.value(interface);
	if(!node){
		if(dynamic_cast<SpriterEngine::SpriteObjectInfo*>(interface)) {
			node = new QSGSpriterObjectNode(interface, window());
		}
		else if(dynamic_cast<SpriterEngine::QtBoneInstanceInfo*>(interface)) {
			node = new QSGSpriterBoneNode(interface);
		}
		else {
			node = new QSGSpriterBase(interface);
		}
		m_nodeMap.insert(interface,node);
	}
	return node;
}

QSGNode *QtEntityInstance::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
	QMutexLocker locker(&m_zOrderMutex);
	QSGNode * node = oldNode;
	if(!m_zOrder || m_zOrder->empty()) {
		delete oldNode;
		return 0;
	}

	if(!node) {
		// First run
		node = new QSGNode();
		node->setFlag(QSGNode::OwnedByParent);
		for(SpriterEngine::UniversalObjectInterface* interface : *m_zOrder) {
			node->appendChildNode(getQSGSpriterNode(interface)->update());
		}
		m_zOrderChanged = false;
	}
	else if(m_zOrderChanged) {
		// Repopulate the base node
		if(true) {
			// Trying to optimize. The node childs are linked list, so pushing them arround should be fast.
			QSGNode* previousNode = nullptr;
			for(SpriterEngine::UniversalObjectInterface* interface : *m_zOrder) {
				QSGSpriterBase* current = getQSGSpriterNode(interface);
				QSGNode* currentNode = current->node();
				if(currentNode) {
					if(previousNode) {
						if(currentNode->previousSibling() != previousNode){
							// If it's not in the right place, put it after the previous
							if(currentNode->parent()) {
								node->removeChildNode(currentNode);
							}
							node->insertChildNodeAfter(currentNode, previousNode);
						}
					}
					else {
						// The node should be added to the front
						if(node->firstChild() != currentNode) { // Only if it's not yet the first
							if(currentNode->parent()) {
								node->removeChildNode(currentNode);
							}
							node->prependChildNode(currentNode);
						}
					}
					current->update();
					previousNode = currentNode;
				}
			}
			if(previousNode) {
				// All nodes are inserted so all after the last ones are no longer needed.
				QSGNode* next = previousNode->nextSibling();
				QSGNode* current = nullptr;
				while(next) {
					current = next;
					next = next->nextSibling();
					node->removeChildNode(current);
				}
			}
		}
		else {
			// Unoptimized way could sometimes be faster.
			node->removeAllChildNodes();
			for(SpriterEngine::UniversalObjectInterface* interface : *m_zOrder) {
				node->appendChildNode(getQSGSpriterNode(interface)->update());
			}
		}
		m_zOrderChanged = false;
	}
	else {
		for(SpriterEngine::UniversalObjectInterface* interface : *m_zOrder) {
			getQSGSpriterNode(interface)->update();
		}
	}
	return node;
}
