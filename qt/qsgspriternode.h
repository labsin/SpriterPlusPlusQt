#ifndef QSGSPRITERNODE_H
#define QSGSPRITERNODE_H

#include <QSGOpacityNode>

namespace SpriterEngine {
class UniversalObjectInterface;
class QtImageFile;
}
class QSGSimpleTextureNode;
class QSGTransformNode;
class QQuickWindow;

class QSGSpriterNode : public QSGOpacityNode
{
public:
		QSGSpriterNode(SpriterEngine::UniversalObjectInterface *interface, QQuickWindow* window);
		~QSGSpriterNode();

		QSGSimpleTextureNode *textureNode() const;

		QSGTransformNode *transNode() const;

		SpriterEngine::QtImageFile *imageFile() const;

		QSGSpriterNode* update();

		SpriterEngine::UniversalObjectInterface *interface() const;
		void setInterface(SpriterEngine::UniversalObjectInterface *interface);

private:
		QSGSimpleTextureNode* m_textureNode;
		QSGTransformNode* m_transNode;
		SpriterEngine::QtImageFile* m_imageFile;
		SpriterEngine::UniversalObjectInterface* m_interface;
};

#endif // QSGSPRITERNODE_H
