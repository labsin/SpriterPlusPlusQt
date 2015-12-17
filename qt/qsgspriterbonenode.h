#ifndef QSGSPRITERBONENODE_H
#define QSGSPRITERBONENODE_H

#include "qsgspriterbase.h"

#include "global/global.h"

#include <QSGTransformNode>

class QSGSpriterBoneNode : public QSGSpriterBase, public QSGTransformNode
{
public:
	QSGSpriterBoneNode(SpriterEngine::UniversalObjectInterface *interface);
    ~QSGSpriterBoneNode();

private:
	void updateGeometry(float scale_x, float scale_y);
	QSGGeometryNode* m_geometryNode;
	QSGGeometry* m_geometry;

	SpriterEngine::point m_initialSize;

	// QSGNode interface
public:
	bool isSubtreeBlocked() const override;

	// QSGSpriterBaseNode interface
public:
	QSGNode *node() override;
	QSGNode *update() override;
};

#endif // QSGSPRITERBONENODE_H
