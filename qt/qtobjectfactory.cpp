#include "qtobjectfactory.h"

#include "qtpointinstanceinfo.h"
#include "qtboxinstanceinfo.h"
#include "qtboneinstanceinfo.h"

using namespace SpriterEngine;

QtObjectFactory::QtObjectFactory()
{
}

PointInstanceInfo *SpriterEngine::QtObjectFactory::newPointInstanceInfo()
{
	return new QtPointInstanceInfo();
}

BoxInstanceInfo *SpriterEngine::QtObjectFactory::newBoxInstanceInfo(SpriterEngine::point size)
{
	return new QtBoxInstanceInfo(size);
}

BoneInstanceInfo *SpriterEngine::QtObjectFactory::newBoneInstanceInfo(SpriterEngine::point size)
{
	return new QtBoneInstanceInfo(size);
}
