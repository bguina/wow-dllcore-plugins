#pragma once

#include "game/WowVector3f.h"

#include "BoundingBox.h"

class RadiusZone {
public:
	RadiusZone();
	RadiusZone(WowVector3f position, float radius);
	virtual ~RadiusZone();

	void setPosition(WowVector3f position);

	bool contains(float x, float y) const;
	BoundingBox getBoundingBox() const;

protected:
	void computeBoundingBox();

	WowVector3f mPosition;
	float mRadius;
	BoundingBox mBoundingBox;
};
