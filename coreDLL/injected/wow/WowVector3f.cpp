#include "pch.h"

#include <cmath>
#include "WowVector3f.h"

const double PI = 3.141592653589793;

WowVector3f::WowVector3f(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {
	if (4 * 3 != sizeof(WowVector3f))
		throw "Illegal struct size.";
}
WowVector3f::WowVector3f() : x(0), y(0), z(0) {}
WowVector3f::WowVector3f(const WowVector3f& copy) : x(copy.x), y(copy.y), z(copy.z) {}

bool WowVector3f::isOrigin() const {
	return .00f == x && .00f == y && .00f == z;
}

float WowVector3f::getDistanceTo(const WowVector3f& to) const {
	auto dx = to.x - x;
	auto dy = to.y - y;
	auto dz = to.z - z;
	auto dx2 = dx * dx;
	auto dy2 = dy * dy;
	auto dz2 = dz * dz;

	return sqrt(dx2 + dy2 + dz2);
}

float WowVector3f::getFlightDistanceTo(const WowVector3f& to) const {
	auto dx = to.x - x;
	auto dy = to.y - y;
	auto dx2 = dx * dx;
	auto dy2 = dy * dy;

	return sqrt(dx2 + dy2);
}

int WowVector3f::getFacingDegreesTo(const WowVector3f& to) const {
	return (int)(atan2(to.y - y, to.x - x) * 180.0 / PI);
}

int WowVector3f::getFacingDeltaDegrees(int angle, const WowVector3f& to) const {
	int targetFacing = getFacingDegreesTo(to);
	int delta = targetFacing - angle;

	if (delta < -180)
		return delta + 360;

	if (delta > 180)
		return delta - 360;

	return delta;
}
