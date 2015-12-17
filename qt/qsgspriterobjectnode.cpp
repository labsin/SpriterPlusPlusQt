#include "qsgspriterobjectnode.h"

#include "objectinfo/universalobjectinterface.h"
#include "qtimagefile.h"
#include "global/global.h"

#include <QSGSimpleTextureNode>
#include <QSGTransformNode>

QSGSpriterObjectNode::QSGSpriterObjectNode(SpriterEngine::UniversalObjectInterface *interface, QQuickWindow *window):
	QSGSpriterBase(interface), QSGOpacityNode()
{
	m_imageFile = dynamic_cast<SpriterEngine::QtImageFile*>(m_interface->getImage());
	Q_ASSERT(m_imageFile);

	QSGTexture *texture = m_imageFile->getTexture(window);
	m_textureNode = new QSGSimpleTextureNode();
	m_textureNode->setRect(m_imageFile->rect());
	m_textureNode->setTexture(texture);

	m_transNode = new QSGTransformNode();
	m_transNode->appendChildNode(m_textureNode);

	appendChildNode(m_transNode);
}

QSGSpriterObjectNode::~QSGSpriterObjectNode()
{
	delete m_textureNode;
	delete m_transNode;
}

QSGNode *QSGSpriterObjectNode::update()
{
	setOpacity(m_interface->getAlpha());

	QMatrix4x4 matrix;
	matrix.translate(m_interface->getPosition().x,
									 m_interface->getPosition().y);
	matrix.rotate(SpriterEngine::toDegrees(m_interface->getAngle()),QVector3D(0,0,1));
	matrix.scale(m_interface->getScale().x,m_interface->getScale().y);
	matrix.translate(-m_interface->getPivot().x*m_imageFile->width(),
									 -m_interface->getPivot().y*m_imageFile->height());
	m_transNode->setMatrix(matrix);
	return this;
}


QSGNode *QSGSpriterObjectNode::node()
{
	return this;
}
