#include "qsgspriterbonenode.h"

#include "objectinfo/universalobjectinterface.h"
#include "global/settings.h"

#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGTransformNode>

QSGSpriterBoneNode::QSGSpriterBoneNode(SpriterEngine::UniversalObjectInterface *interface):
	QSGSpriterBase(interface), QSGTransformNode()
{
	m_initialSize = interface->getSize();
	m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
	m_geometry->setDrawingMode(GL_TRIANGLE_FAN);
	//m_geometry->setLineWidth(3);
	m_geometry->vertexDataAsPoint2D()[0].set(0, m_initialSize.y/2.0);
	m_geometry->vertexDataAsPoint2D()[1].set(4, 0);
	m_geometry->vertexDataAsPoint2D()[2].set(m_initialSize.x, m_initialSize.y/2.0);
	m_geometry->vertexDataAsPoint2D()[3].set(4, m_initialSize.y);

	QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
	material->setColor(QColor(0, 0, 255, 127));

	m_geometryNode = new QSGGeometryNode;
	m_geometryNode->setGeometry(m_geometry);
	m_geometryNode->setMaterial(material);
	m_geometryNode->setFlag(QSGNode::OwnsMaterial);

	appendChildNode(m_geometryNode);
}

QSGSpriterBoneNode::~QSGSpriterBoneNode()
{
	delete m_geometry;
	delete m_geometryNode;
}

QSGNode *QSGSpriterBoneNode::update()
{
	//updateGeometry(m_interface->getScale().x,m_interface->getScale().y);
	QMatrix4x4 matrix;
	matrix.translate(m_interface->getPosition().x,
									 m_interface->getPosition().y);
	matrix.rotate(SpriterEngine::toDegrees(m_interface->getAngle()),QVector3D(0,0,1));
	matrix.scale(m_interface->getScale().x,m_interface->getScale().y);
	matrix.translate(0, -m_initialSize.y*m_interface->getScale().y/2.0);
	setMatrix(matrix);
	return this;
}

void QSGSpriterBoneNode::updateGeometry(float scale_x, float scale_y)
{
	m_geometry->vertexDataAsPoint2D()[0].set(0, m_initialSize.y*scale_y/2.0);
	m_geometry->vertexDataAsPoint2D()[1].set(4, 0);
	m_geometry->vertexDataAsPoint2D()[2].set(m_initialSize.x*scale_x, m_initialSize.y*scale_y/2.0);
	m_geometry->vertexDataAsPoint2D()[3].set(4, m_initialSize.y*scale_y);
}


bool QSGSpriterBoneNode::isSubtreeBlocked() const
{
	if(!SpriterEngine::Settings::renderDebugBones) {
		return true;
	}
	return false;
}


QSGNode *QSGSpriterBoneNode::node()
{
	return this;
}
