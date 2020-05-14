#pragma once

#include <cstdint>

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