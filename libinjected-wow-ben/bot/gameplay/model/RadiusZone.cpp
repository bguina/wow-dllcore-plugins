#include "RadiusZone.h"

RadiusZone::RadiusZone() : RadiusZone(WowVector3f(), .0f) {

}

RadiusZone::RadiusZone(WowVector3f position, float radius) :
	mPosition(position),
	mRadius(radius)
{
	computeBoundingBox();
}

RadiusZone::~RadiusZone() = default;

void RadiusZone::setPosition(WowVector3f position) {
	mPosition = position;
	computeBoundingBox();
}

bool RadiusZone::contains(float x, float y) const
{
	return mBoundingBox.contains(x, y);
}

BoundingBox RadiusZone::getBoundingBox() const {
	return mBoundingBox;
}

void RadiusZone::computeBoundingBox() {
	mBoundingBox = BoundingBox(
		mPosition.translateByX(-mRadius).translateByY(-mRadius).translateByZ(-mRadius),
		mPosition.translateByX(mRadius).translateByY(mRadius).translateByZ(mRadius)
	);
}
