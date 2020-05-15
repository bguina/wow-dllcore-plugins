#include "pch.h"

#include <cmath>
#include "Vector3f.h"

const double PI = 3.141592653589793;


int getVectorFacingDegrees(
	const Vector3f& from,
	const Vector3f& to
) {
	return atan2(from.y - to.y, to.x - from.x) * 180 / PI;
}

int getVectorFacingDeltaDegrees(
	const Vector3f& from,
	int angle,
	const Vector3f& to
) {
	int targetFacing = getVectorFacingDegrees(from, to);
	int delta = targetFacing - angle;

	if (delta < -180)
		return delta + 360;
	else if (delta > 180)
		return delta - 360;
	
	return delta;
}
