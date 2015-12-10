#include "qentityinstance.h"
#include "qtimagefile.h"

#include <entity/entityinstance.h>
#include <objectinfo/universalobjectinterface.h>
#include <objectinfo/spriteobjectinfo.h>
#include <global/settings.h>

#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QSGTransformNode>

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
		connect(window,SIGNAL(afterAnimating()),this,SLOT(updateInterface()));
		connect(window,SIGNAL(frameSwapped()),this,SLOT(updateIfLoaded()));
		updateIfLoaded();
	}
}

void QEntityInstance::updateIfLoaded()
{
	if(!m_loaded)
		return;
	m_entity->setTimeElapsed(m_time.restart());
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

void QEntityInstance::updateInterface()
{
	using namespace SpriterEngine;

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
	QSGNode * node = oldNode;
	if(m_interfaces.empty()) {
		delete oldNode;
		return 0;
	}
	if(!node || m_zOrderChanged) {
		delete node;
		node = new QSGNode();
		m_zOrderChanged = false;
		for(SpriterEngine::UniversalObjectInterface* interface : m_interfaces) {
			SpriterEngine::QtImageFile* imageFile = dynamic_cast<SpriterEngine::QtImageFile*>(interface->getImage());
			QSGSimpleTextureNode* childNode = new QSGSimpleTextureNode();
			childNode->setFlag(QSGNode::OwnedByParent);
			QSGTexture *texture = imageFile->getTexture(window());
			childNode->setRect(imageFile->rect());
			childNode->setTexture(texture);
			QMatrix4x4 matrix;
			matrix.translate(interface->getPosition().x,
							 interface->getPosition().y);
			matrix.scale(interface->getScale().x,interface->getScale().y);
			matrix.rotate(SpriterEngine::toDegrees(interface->getAngle()),QVector3D(0,0,1));
			matrix.translate(-interface->getPivot().x*imageFile->width(),
							 -interface->getPivot().y*imageFile->height());
			QSGTransformNode* transNote = new QSGTransformNode();
			transNote->setFlag(QSGNode::OwnedByParent);
			transNote->setMatrix(matrix);
			transNote->appendChildNode(childNode);
			node->appendChildNode(transNote);
		}
	}
	else {
		int index=0;
		for(SpriterEngine::UniversalObjectInterface* interface : m_interfaces) {
			QSGTransformNode* transNote = dynamic_cast<QSGTransformNode*>(node->childAtIndex(index));
			SpriterEngine::QtImageFile* imageFile = dynamic_cast<SpriterEngine::QtImageFile*>(interface->getImage());
			QMatrix4x4 matrix;
			matrix.translate(interface->getPosition().x,
							 interface->getPosition().y);
			matrix.scale(interface->getScale().x,interface->getScale().y);
			matrix.rotate(SpriterEngine::toDegrees(interface->getAngle()),QVector3D(0,0,1));
			matrix.translate(-interface->getPivot().x*imageFile->width(),
							 -interface->getPivot().y*imageFile->height());
			transNote->setMatrix(matrix);
			index++;
		}
	}
	return node;
}
