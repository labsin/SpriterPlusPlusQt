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
	QSGNode *node() override;
	QSGNode* update() override;

private:
	QSGSimpleTextureNode* m_textureNode;
	QSGTransformNode* m_transNode;
	SpriterEngine::QtImageFile* m_imageFile;
	bool m_rotate;
};

#endif // QSGSPRITEROBJECTNODE_H
