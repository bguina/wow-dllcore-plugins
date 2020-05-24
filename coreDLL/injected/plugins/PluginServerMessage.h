#pragma once

#include <vector>

#include "MessageType.h"

#include "../../pathfinder/Vector3f.h"

class PluginServerMessage {
public:
	MessageType type;
	union {
		char eject;
		const std::vector<Vector3f>* waypoints;
	} data;
};
