#pragma once

#include <fstream>

extern const double PI;

struct WowVector3f {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float coord[3];
	};

	WowVector3f();
	WowVector3f(float x1, float y1, float z1);
	WowVector3f(const WowVector3f& copy);

	bool isOrigin() const;

	float getDistanceTo(const WowVector3f& to) const;
	float getFlightDistanceTo(const WowVector3f& to) const;

	int getFacingDegreesTo(const WowVector3f& to) const;
	int getFacingDeltaDegrees(int angle, const WowVector3f& to) const;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowVector3f& obj
	)
{
	out << "[Vector3f:" << obj.x << "/" << obj.y << "/" << obj.z << "]";
	return out;
}
