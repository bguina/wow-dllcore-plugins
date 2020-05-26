#pragma once

#include <vector>

#include "MessageType.h"

#include "../../injectable/wow/game/WowVector3f.h"

class ClientMessage {
public:
	MessageType type;
	char eject;
	const std::vector<WowVector3f>* waypoints;
};
