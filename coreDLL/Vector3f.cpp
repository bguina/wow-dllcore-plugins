#include "pch.h"

#include <cmath>
#include "Vector3f.h"

const double PI = 3.141592653589793;

bool Vector3f::isOrigin() const {
	return .00f == x && .00f == y && .00 == z;
}

float Vector3f::getDistanceTo(const Vector3f& to) const {
	auto dx = to.x - x;
	auto dy = to.y - y;
	auto dz = to.z - z;
	auto dx2 = dx * dx;
	auto dy2 = dy * dy;
	auto dz2 = dz * dz;

	return sqrt(dx2 + dy2 + dz2);
}

float Vector3f::getFlightDistanceTo(const Vector3f& to) const {
	auto dx = to.x - x;
	auto dy = to.y - y;
	auto dx2 = dx * dx;
	auto dy2 = dy * dy;

	return sqrt(dx2 + dy2);
}

int Vector3f::getFacingDegreesTo(const Vector3f& to) const {
	return (int)(atan2(to.y - y, to.x - x) * 180.0 / PI);
}

int Vector3f::getFacingDeltaDegrees(int angle, const Vector3f& to) const {
	int targetFacing = getFacingDegreesTo(to);
	int delta = targetFacing - angle;

	if (delta < -180)
		return delta + 360;

	if (delta > 180)
		return delta - 360;

	return delta;
}
