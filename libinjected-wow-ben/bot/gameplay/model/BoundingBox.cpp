
#include "BoundingBox.h"

BoundingBox::BoundingBox() : BoundingBox(WOWVECTOR3F_ORIGIN, WOWVECTOR3F_ORIGIN) {

}

BoundingBox::BoundingBox(const WowVector3f start, const WowVector3f end) :
	mStart(start),
	mEnd(end)
{
}

bool BoundingBox::contains(const float x, const float y) const
{
	return contains({ x, y, (mStart.position.z + mEnd.position.z) / 2 });
}

bool BoundingBox::contains(WowVector3f v) const
{
	if (mStart.position.x <= v.position.x && v.position.x <= mEnd.position.x && mStart.position.y <= v.position.y && v.position.y <= mEnd.position.y) {
		// Point is in bounding box
		return true;
	}
	return false; // fixme
}
