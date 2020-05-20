#pragma once

#include <fstream>

extern const double PI;

struct Vector3f {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float coord[3];
	};

	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
	Vector3f(const Vector3f& copy) : x(copy.x), y(copy.y), z(copy.z) {}

	bool isOrigin() const;

	float getDistanceTo(const Vector3f& to) const;
	float getFlightDistanceTo(const Vector3f& to) const;

	int getFacingDegreesTo(const Vector3f& to) const;
	int getFacingDeltaDegrees(int angle, const Vector3f& to) const;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const struct Vector3f& obj
	)
{
	out << "[Vector3f:" << obj.x << "/" << obj.y << "/" << obj.z << "]";
	return out;
}
