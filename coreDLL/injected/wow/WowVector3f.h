#pragma once

#include <fstream>

extern const double PI;

struct WowVector3f {
	union {
		struct {
			float x;
			float y;
			float z;
		} position;
		float matrix[3] = { .0f,.0f,.0f };
	};

	float getDistanceTo(const WowVector3f& to) const;
	float getFlightDistanceTo(const WowVector3f& to) const;

	int getFacingDegreesTo(const WowVector3f& to) const;
	int getFacingDeltaDegrees(int angle, const WowVector3f& to) const;

	float operator[](int index) const;

};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowVector3f& obj
	)
{
	out << "[Vector3f:" << obj.position.x << "/" << obj.position.y << "/" << obj.position.z << "]";
	return out;
}
