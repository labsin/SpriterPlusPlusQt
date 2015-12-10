#ifndef QTBOXINSTANCEINFO_H
#define QTBOXINSTANCEINFO_H

#include "objectinfo/boxinstanceinfo.h"

namespace SpriterEngine {
class QtBoxInstanceInfo : public BoxInstanceInfo
{
public:
	QtBoxInstanceInfo(point initialSize);

	void render() override;

private:
};
}

#endif // QTBOXINSTANCEINFO_H
