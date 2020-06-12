#pragma once

#include "game/world/WowVector3f.h"

class BoundingBox {
public:
	BoundingBox();

	BoundingBox(WowVector3f start, WowVector3f end);

	bool contains(float x, float y) const;
	bool contains(WowVector3f v) const;

protected:
	WowVector3f mStart;
	WowVector3f mEnd;
};
