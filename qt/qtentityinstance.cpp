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

QtEntityInstance::~QtEntityInstance()
{
	delete m_entity;
	QMutexLocker locker(&m_nodeMapMutex);
	for(QHash<SpriterEngine::UniversalObjectInterface*, QSGSpriterBase*>::iterator it = m_nodeMap.begin(), end = m_nodeMap.end(); it != end; it++) {
		delete it.value();
	}
}

void QtEntityInstance::setName(QString name)
{
	if (m_name == name)
		return;

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
	if(!m_loaded)
		return;
	updateInterface();
	update();
}

void QtEntityInstance::setAnimation(QString animation)
{
	if (m_animation == animation)
		return;

	m_animation = animation;
	if(m_loaded) {
		m_entity->setCurrentAnimation(m_animation.toStdString());
	}
	emit animationChanged(animation);
}

void QtEntityInstance::setScale(QPointF scale)
{
	if (m_scale == scale)
		return;

	m_scale = scale;
	if(m_loaded) {
		m_entity->setScale(SpriterEngine::point(m_scale.x(),m_scale.y()));
	}
	emit scaleChanged(scale);
}

void QtEntityInstance::setSpeedRatio(float speedRatio)
{
	if (m_speedRatio == speedRatio)
		return;

	m_speedRatio = speedRatio;
	if(m_loaded) {
		m_entity->setPlaybackSpeedRatio(m_speedRatio);
	}
	emit speedRatioChanged(speedRatio);
}


void QtEntityInstance::load()
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

void QtEntityInstance::unload()
{
	delete m_entity;
	m_entity = nullptr;

	m_loaded = false;
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
	using namespace SpriterEngine;
	m_entity->setTimeElapsed(m_time.restart());

	if(!m_loaded)
		return;

	ObjectInterfaceVector* zOrder = m_entity->getZOrder();

	if(zOrder == m_previousZOrder) {
		return;
	}

	m_zOrderChanged = true;
	m_previousZOrder = zOrder;
}


QSGNode *QtEntityInstance::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
	QMutexLocker locker(&m_nodeMapMutex);
	QSGNode * node = oldNode;
	if(!m_previousZOrder || m_previousZOrder->empty()) {
		delete oldNode;
		return 0;
	}

	if(!node) {
		node = new QSGNode();
		node->setFlag(QSGNode::OwnedByParent);
		m_zOrderChanged = false;
		for(SpriterEngine::UniversalObjectInterface* interface : *m_previousZOrder) {
			node->appendChildNode(getQSGSpriterNode(interface)->update());
		}
	}
	else if(m_zOrderChanged) {
		QSGSpriterBase* previousBase = nullptr;
		for(SpriterEngine::UniversalObjectInterface* interface : *m_previousZOrder) {
			QSGSpriterBase* currentBase = getQSGSpriterNode(interface);
			if(currentBase->node()) {
				if(currentBase->node()->parent()) {
					node->removeChildNode(currentBase->node());
				}
				if(previousBase) {
					node->insertChildNodeAfter(currentBase->node(),previousBase->node());
				}
				else {
					node->prependChildNode(currentBase->node());
				}
				currentBase->update();
				previousBase = currentBase;
			}
		}
		while(QSGNode* next = previousBase->node()->nextSibling()) {
			node->removeChildNode(next);
		}
	}
	else {
		for(SpriterEngine::UniversalObjectInterface* interface : *m_previousZOrder) {
			getQSGSpriterNode(interface)->update();
		}
	}
	return node;
}
