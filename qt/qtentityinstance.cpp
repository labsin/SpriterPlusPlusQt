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

QtEntityInstance::QtEntityInstance(QQuickItem *parent):
	QQuickItem(parent),
	m_zOrderChanged(false),
	m_model(nullptr),
	m_currentEntity(nullptr),
	m_zOrder(nullptr),
	m_scale(1,1),
	m_speedRatio(1.0)
{
	setFlag(ItemHasContents);
	connect(this,SIGNAL(windowChanged(QQuickWindow*)),this,SLOT(updateQQuickWindow(QQuickWindow*)));
	updateQQuickWindow(window());
}

QtEntityInstance::~QtEntityInstance()
{
	for(auto it = m_entityMap.begin(), end = m_entityMap.end(); it != end; it++) {
		delete it.value();
	}

	QMutexLocker locker(&m_nodeMapMutex);
	for(auto it = m_nodeMap.begin(), end = m_nodeMap.end(); it != end; it++) {
		delete it.value();
	}
}

void QtEntityInstance::setName(QString name)
{
	if (m_name == name)
		return;

	if(!m_name.isNull()) {
		unload();
	}

	m_name = name;
	if(m_model) {
		load();
	}
	emit nameChanged(name);
}

void QtEntityInstance::setModel(QtSpriterModel *model)
{
	if (m_model == model)
		return;

	m_model = model;
	if(!m_name.isEmpty()) {
		load();
	}
	emit modelChanged(model);
}

void QtEntityInstance::updateQQuickWindow(QQuickWindow *window)
{
	if(window) {
		connect(window,SIGNAL(frameSwapped()),this,SLOT(updateIfLoaded()));
		updateIfLoaded();
	}
}

void QtEntityInstance::updateIfLoaded()
{
	if(!m_currentEntity)
		return;
	updateInterface();
	update();
}

void QtEntityInstance::setAnimation(QString animation)
{
	if (m_animation == animation)
		return;

	m_animation = animation;
	if(m_currentEntity) {
		m_currentEntity->setCurrentAnimation(m_animation.toStdString());
	}
	emit animationChanged(animation);
}

void QtEntityInstance::setScale(QPointF scale)
{
	if (m_scale == scale)
		return;

	m_scale = scale;
	if(m_currentEntity) {
		m_currentEntity->setScale(SpriterEngine::point{m_scale.x(),m_scale.y()});
	}
	emit scaleChanged(scale);
}

void QtEntityInstance::setSpeedRatio(float speedRatio)
{
	if (m_speedRatio == speedRatio)
		return;

	m_speedRatio = speedRatio;
	if(m_currentEntity) {
		m_currentEntity->setPlaybackSpeedRatio(m_speedRatio);
	}
	emit speedRatioChanged(speedRatio);
}


void QtEntityInstance::load()
{
	Q_ASSERT(m_model);
	Q_ASSERT(!m_name.isEmpty());

	m_currentEntity = m_entityMap.value(m_name);
	if(!m_currentEntity) {
		m_currentEntity = m_model->getNewEntityInstance(m_name);
		if(!m_currentEntity) {
			setErrorString("Entity "+m_name+" not found.");
			unload();
			return;
		}
		m_entityMap.insert(m_name, m_currentEntity);
	}
	if(!m_animation.isEmpty()) {
		m_currentEntity->setCurrentAnimation(m_animation.toStdString());
	}
	m_currentEntity->setScale(SpriterEngine::point{m_scale.x(),m_scale.y()});
	m_currentEntity->setPlaybackSpeedRatio(m_speedRatio);

	m_time.start();
}

void QtEntityInstance::unload()
{
	if(m_currentEntity)
		m_currentEntity->pausePlayback();
	m_currentEntity = nullptr;
}

void QtEntityInstance::setErrorString(QString error)
{
	if (m_errorString == error)
		return;

	m_errorString = error;
	emit errorStringChanged(error);
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

void QtEntityInstance::updateInterface()
{
	if(!m_currentEntity)
		return;

	using namespace SpriterEngine;
	m_currentEntity->setTimeElapsed(m_time.restart());

	ObjectInterfaceVector* zOrder = m_currentEntity->getZOrder();

	// std::vector compares sequencially
	if(zOrder == m_zOrder) {
		return;
	}

	m_zOrderChanged = true;
	m_zOrder = zOrder;
	if(m_currentEntity->animationJustFinished()) {
		emit finished();
	}
	if(m_currentEntity->animationJustLooped()) {
		emit looped();
	}
}

void QtEntityInstance::startResume()
{
	if(!m_currentEntity)
		return;

	if(m_animation.isEmpty())
		return;

	m_currentEntity->startResumePlayback();
}

void QtEntityInstance::pause()
{
	if(!m_currentEntity)
		return;

	if(m_animation.isEmpty())
		return;

	m_currentEntity->pausePlayback();
}


QSGNode *QtEntityInstance::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
	QMutexLocker locker(&m_nodeMapMutex);
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
