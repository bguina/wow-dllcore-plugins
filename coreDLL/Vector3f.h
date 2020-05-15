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
};

int getVectorFacingDegrees(
	const Vector3f& from,
	const Vector3f& to
);

int getVectorFacingDeltaDegrees(
	const Vector3f& from,
	int angle,
	const Vector3f& to
);

inline std::ostream& operator<<(
	std::ostream& out,
	const struct Vector3f& obj
	)
{
	out << "[Vector3f:" << obj.x << "/" << obj.y << "/" << obj.z << "]";
	return out;
}
