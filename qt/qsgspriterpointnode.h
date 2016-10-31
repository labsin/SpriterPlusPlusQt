#ifndef QSGSPRITERPOINTNODE_H
#define QSGSPRITERPOINTNODE_H

#include "qsgspriterbase.h"

#include "global/global.h"

#include <QSGGeometryNode>

class QSGSpriterPointNode : public QSGSpriterBase, public QSGGeometryNode
{
public:
	QSGSpriterPointNode(SpriterEngine::UniversalObjectInterface *interface);
	~QSGSpriterPointNode();

private:
	QSGGeometry* m_geometry;

	// QSGNode interface
public:
	bool isSubtreeBlocked() const override;

	// QSGSpriterBaseNode interface
public:
	QSGNode *node() override;
	QSGNode *update() override;
};

#endif // QSGSPRITERPOINTNODE_H
