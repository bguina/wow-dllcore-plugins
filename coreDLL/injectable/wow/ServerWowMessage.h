#pragma once

#include <vector>
#include <memory>

#include "MessageType.h"
#include "Client.h"

#include "game/WowVector3f.h"

class ServerWowMessage {
public:
	ServerWowMessage(Client* client) :
		cl(client),
		type(MessageType::UNKNOWN),
		eject(false),
		waypoints(nullptr),
		subscriptions(nullptr)
	{

	}

	Client* cl;
	MessageType type;
	char eject;
	const std::vector<WowVector3f>* waypoints;
	const std::vector<std::string>* subscriptions;
};
