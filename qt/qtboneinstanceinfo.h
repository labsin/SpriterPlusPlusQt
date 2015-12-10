#ifndef QTBONEINSTANCEINFO_H
#define QTBONEINSTANCEINFO_H

#include "objectinfo/boneinstanceinfo.h"

namespace SpriterEngine {
class QtBoneInstanceInfo : public BoneInstanceInfo
{
public:
	QtBoneInstanceInfo(point initialSize);

	void render() override;
private:
};
}

#endif // QTBONEINSTANCEINFO_H
