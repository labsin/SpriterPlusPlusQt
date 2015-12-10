#ifndef QTPOINTINSTANCEINFO_H
#define QTPOINTINSTANCEINFO_H

#include "objectinfo/pointinstanceinfo.h"

namespace SpriterEngine {
class QtPointInstanceInfo : public PointInstanceInfo
{
public:
	QtPointInstanceInfo();

	void render() override;

private:
};
}

#endif // QTPOINTINSTANCEINFO_H
