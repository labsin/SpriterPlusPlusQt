#include "qsgspriterpointnode.h"

#include "objectinfo/universalobjectinterface.h"
#include "global/settings.h"

#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <QSGTransformNode>

QSGSpriterPointNode::QSGSpriterPointNode(SpriterEngine::UniversalObjectInterface *interface):
	QSGSpriterBase(interface), QSGGeometryNode()
{
	m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 1);
	m_geometry->setDrawingMode(GL_POINT);
	m_geometry->setLineWidth(3);
	m_geometry->vertexDataAsPoint2D()[0].set(0, 0);

	QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
	material->setColor(QColor(0, 255, 255, 127));

	setGeometry(m_geometry);
	setMaterial(material);
	setFlag(QSGNode::OwnsMaterial);;
}

QSGSpriterPointNode::~QSGSpriterPointNode()
{
	delete m_geometry;
}

QSGNode *QSGSpriterPointNode::update()
{
	//updateGeometry(m_interface->getScale().x,m_interface->getScale().y);
	m_geometry->vertexDataAsPoint2D()[0].set(m_interface->getPosition().x, m_interface->getPosition().y);
	m_geometry->markVertexDataDirty();
	return this;
}


bool QSGSpriterPointNode::isSubtreeBlocked() const
{
	if(!SpriterEngine::Settings::renderDebugPoints) {
		return true;
	}
	return false;
}


QSGNode *QSGSpriterPointNode::node()
{
	return this;
}
