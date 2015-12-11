#include "qentityinstance.h"
#include "qtimagefile.h"
#include "qsgspriternode.h"

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

QEntityInstance::QEntityInstance(QQuickItem *parent):
	QQuickItem(parent),
	m_zOrderChanged(false),
	m_loaded(false),
	m_model(nullptr),
	m_entity(nullptr),
	m_previousZOrder(nullptr),
	m_scale(1,1),
	m_speedRatio(1.0)
{
	setFlag(ItemHasContents);
	connect(this,SIGNAL(windowChanged(QQuickWindow*)),this,SLOT(updateQQuickWindow(QQuickWindow*)));
	updateQQuickWindow(window());
}

QEntityInstance::~QEntityInstance()
{
	delete m_entity;
	QMutexLocker locker(&m_nodeMapMutex);
	for(QHash<SpriterEngine::UniversalObjectInterface*, QSGSpriterNode*>::iterator it = m_nodeMap.begin(), end = m_nodeMap.end(); it != end; it++) {
		delete it.value();
	}
}

void QEntityInstance::setName(QString name)
{
	if (m_name == name)
		return;

	m_name = name;
	if(m_model) {
		load();
	}
	emit nameChanged(name);
}

void QEntityInstance::setModel(QSprinterModel *model)
{
	if (m_model == model)
		return;

	m_model = model;
	if(!m_name.isEmpty()) {
		load();
	}
	emit modelChanged(model);
}

void QEntityInstance::updateQQuickWindow(QQuickWindow *window)
{
	if(window) {
		connect(window,SIGNAL(frameSwapped()),this,SLOT(updateIfLoaded()));
		updateIfLoaded();
	}
}

void QEntityInstance::updateIfLoaded()
{
	if(!m_loaded)
		return;
	updateInterface();
	update();
}

void QEntityInstance::setAnimation(QString animation)
{
	if (m_animation == animation)
		return;

	m_animation = animation;
	if(m_loaded) {
		m_entity->setCurrentAnimation(m_animation.toStdString());
	}
	emit animationChanged(animation);
}

void QEntityInstance::setScale(QPointF scale)
{
	if (m_scale == scale)
		return;

	m_scale = scale;
	if(m_loaded) {
		m_entity->setScale(SpriterEngine::point(m_scale.x(),m_scale.y()));
	}
	emit scaleChanged(scale);
}

void QEntityInstance::setSpeedRatio(float speedRatio)
{
	if (m_speedRatio == speedRatio)
		return;

	m_speedRatio = speedRatio;
	if(m_loaded) {
		m_entity->setPlaybackSpeedRatio(m_speedRatio);
	}
	emit speedRatioChanged(speedRatio);
}


void QEntityInstance::load()
{
	Q_ASSERT(m_model);
	Q_ASSERT(!m_name.isEmpty());

	m_entity = m_model->getNewEntityInstance(m_name);
	if(!m_animation.isEmpty()) {
		m_entity->setCurrentAnimation(m_animation.toStdString());
	}
	m_entity->setScale(SpriterEngine::point(m_scale.x(),m_scale.y()));
	m_entity->setPlaybackSpeedRatio(m_speedRatio);

	m_loaded = true;
	m_time.start();
}

void QEntityInstance::unload()
{
	delete m_entity;
	m_entity = nullptr;

	m_loaded = false;
}

QSGSpriterNode *QEntityInstance::getQSGSpriterNode(SpriterEngine::UniversalObjectInterface *interface)
{
	QSGSpriterNode* node = m_nodeMap.value(interface);
	if(!node){
		node = new QSGSpriterNode(interface, window());
		m_nodeMap.insert(interface,node);
	}
	return node;
}

void QEntityInstance::updateInterface()
{
	using namespace SpriterEngine;
	m_entity->setTimeElapsed(m_time.restart());

	if(!m_loaded)
		return;

	ObjectInterfaceVector* zOrder = m_entity->getZOrder();

	if(zOrder == m_previousZOrder) {
		return;
	}

	m_interfaces.clear();

	if (zOrder)	{
		for (UniversalObjectInterface* it : *zOrder) {
			if(dynamic_cast<QtImageFile*>(it->getImage())) {
				m_interfaces.push_back(it);
			}
		}
	}

	m_zOrderChanged = true;
	m_previousZOrder = zOrder;
}


QSGNode *QEntityInstance::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
	QMutexLocker locker(&m_nodeMapMutex);
	QSGNode * node = oldNode;
	if(m_interfaces.empty()) {
		delete oldNode;
		return 0;
	}

	if(!node) {
		node = new QSGNode();
		node->setFlag(QSGNode::OwnedByParent);
		m_zOrderChanged = false;
		for(SpriterEngine::UniversalObjectInterface* interface : m_interfaces) {
			node->appendChildNode(getQSGSpriterNode(interface)->update());
		}
	}
	else if(m_zOrderChanged) {
		QSGSpriterNode* previousNode = nullptr;
		for(SpriterEngine::UniversalObjectInterface* interface : m_interfaces) {
			QSGSpriterNode* currentNode = getQSGSpriterNode(interface);
			if(currentNode->parent()) {
				node->removeChildNode(currentNode);
			}
			if(previousNode) {
				node->insertChildNodeAfter(currentNode,previousNode);
			}
			else {
				node->prependChildNode(currentNode);
			}
			currentNode->update();
			previousNode = currentNode;
		}
		while(QSGNode* next = previousNode->nextSibling()) {
			node->removeChildNode(next);
		}
	}
	else {
		for(SpriterEngine::UniversalObjectInterface* interface : m_interfaces) {
			getQSGSpriterNode(interface)->update();
		}
	}
	return node;
}
