#ifndef QTOBJECTFACTORY_H
#define QTOBJECTFACTORY_H

#include <override/objectfactory.h>
#include <objectinfo/pointobjectinfo.h>

namespace SpriterEngine {
class QtObjectFactory : public ObjectFactory
{
public:
	QtObjectFactory();

	PointInstanceInfo *newPointInstanceInfo() override;

	BoxInstanceInfo *newBoxInstanceInfo(point size) override;

	BoneInstanceInfo *newBoneInstanceInfo(point size) override;

};
}

#endif // QTOBJECTFACTORY_H
