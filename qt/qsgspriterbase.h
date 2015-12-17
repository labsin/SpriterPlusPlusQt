#ifndef QSGSPRITERBASE_H
#define QSGSPRITERBASE_H

namespace SpriterEngine {
class UniversalObjectInterface;
}
class QSGNode;

class QSGSpriterBase
{
public:
	QSGSpriterBase(SpriterEngine::UniversalObjectInterface *interface);
	virtual ~QSGSpriterBase();

	virtual QSGNode *update();

	virtual QSGNode* node();

	SpriterEngine::UniversalObjectInterface *interface() const;

protected:
	SpriterEngine::UniversalObjectInterface* m_interface;
};

#endif // QSGSPRITERBASE_H
