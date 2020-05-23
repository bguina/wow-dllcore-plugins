#pragma once

#include <vector>

#include "Vector3f.h"

class IPathWaypointsConsumer 
{
public:
	virtual void loadPathWaypoints(const std::vector<Vector3f>& waypoints) = 0;
};
