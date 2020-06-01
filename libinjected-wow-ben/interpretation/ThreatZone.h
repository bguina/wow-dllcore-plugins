#pragma once

#include "game/WowVector3f.h"

typedef float ThreatPercent;

struct BoundingBox {
public:
	BoundingBox() : BoundingBox(WowVector3f(), WowVector3f()) {

	}

	BoundingBox(WowVector3f start, WowVector3f end) : mStart(start), mEnd(end) {

	}

	bool contains(WowVector3f position) {

	}

protected:
	WowVector3f mStart;
	WowVector3f mEnd;
};

class IBoundedZone {
public:
	virtual BoundingBox getBoundaryBox() = 0;
};

class RadiusZone : public IBoundedZone {
public:
	RadiusZone() : RadiusZone(WowVector3f(), .0f) {

	}

	RadiusZone(WowVector3f position, float radius) :mPosition(position), mRadius(radius) {
		computeBoundingBox();
	}

	void setPosition(WowVector3f position) {
		mPosition = position;
		computeBoundingBox();
	}

	virtual BoundingBox getBoundaryBox() {
		return mBoundingBox;
	}

protected:
	void computeBoundingBox() {
		mBoundingBox = BoundingBox(
			mPosition.translateByX(-mRadius).translateByY(-mRadius),
			mPosition.translateByX(mRadius).translateByY(mRadius)
		);
	}

	WowVector3f mPosition;
	float mRadius;
	BoundingBox mBoundingBox;
};

struct RadiusThreatZone : public RadiusZone {
public:

protected:
	ThreatPercent mThreat;
};

struct UnitAggroZone : public RadiusThreatZone {
public:

protected:
	WowGuid128 mUnit;
	RadiusThreatZone mAggro;
};

struct DangerZone {
public:

protected:
	std::list<UnitAggroZone> units;
	ThreatPercent threat;
	WowVector3f boundaryStart;
	WowVector3f boundaryEnd;
};

