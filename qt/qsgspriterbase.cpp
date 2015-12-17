#include "qsgspriterbase.h"

#include "objectinfo/universalobjectinterface.h"

QSGSpriterBase::QSGSpriterBase(SpriterEngine::UniversalObjectInterface *interface):
	m_interface(interface)
{
}

QSGSpriterBase::~QSGSpriterBase()
{
}

QSGNode *QSGSpriterBase::update()
{
	return node();
}

QSGNode *QSGSpriterBase::node()
{
	return nullptr;
}

SpriterEngine::UniversalObjectInterface *QSGSpriterBase::interface() const
{
	return m_interface;
}
