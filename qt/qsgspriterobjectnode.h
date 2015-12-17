#ifndef QSGSPRITEROBJECTNODE_H
#define QSGSPRITEROBJECTNODE_H

#include "qsgspriterbase.h"

#include <QSGOpacityNode>

class QSGSimpleTextureNode;
class QQuickWindow;
namespace SpriterEngine {
class QtImageFile;
}

class QSGSpriterObjectNode : public QSGSpriterBase, public QSGOpacityNode
{
public:
	QSGSpriterObjectNode(SpriterEngine::UniversalObjectInterface *interface, QQuickWindow* window);
	~QSGSpriterObjectNode();

private:
	QSGSimpleTextureNode* m_textureNode;
	QSGTransformNode* m_transNode;
	SpriterEngine::QtImageFile* m_imageFile;

	// QSGSpriterBaseNode interface
public:
	QSGNode *node() override;
	QSGNode* update() override;
};

#endif // QSGSPRITEROBJECTNODE_H
