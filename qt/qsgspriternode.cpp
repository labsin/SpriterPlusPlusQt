#include "qsgspriternode.h"
#include "objectinfo/universalobjectinterface.h"
#include "qtimagefile.h"
#include "global/global.h"

#include <QSGSimpleTextureNode>
#include <QSGTransformNode>

QSGSpriterNode::QSGSpriterNode(SpriterEngine::UniversalObjectInterface *interface, QQuickWindow* window):
	QSGOpacityNode(), m_interface(interface)
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

QSGSpriterNode::~QSGSpriterNode()
{
	delete m_textureNode;
	delete m_transNode;
}

QSGSimpleTextureNode *QSGSpriterNode::textureNode() const
{
	return m_textureNode;
}

QSGTransformNode *QSGSpriterNode::transNode() const
{
	return m_transNode;
}

SpriterEngine::QtImageFile *QSGSpriterNode::imageFile() const
{
	return m_imageFile;
}

QSGSpriterNode *QSGSpriterNode::update(){
	setOpacity(m_interface->getAlpha());

	QMatrix4x4 matrix;
	matrix.translate(m_interface->getPosition().x,
									 m_interface->getPosition().y);
	matrix.scale(m_interface->getScale().x,m_interface->getScale().y);
	matrix.rotate(SpriterEngine::toDegrees(m_interface->getAngle()),QVector3D(0,0,1));
	matrix.translate(-m_interface->getPivot().x*m_imageFile->width(),
									 -m_interface->getPivot().y*m_imageFile->height());
	m_transNode->setMatrix(matrix);
	return this;
}

SpriterEngine::UniversalObjectInterface *QSGSpriterNode::interface() const
{
	return m_interface;
}

void QSGSpriterNode::setInterface(SpriterEngine::UniversalObjectInterface *interface)
{
	m_interface = interface;
}
