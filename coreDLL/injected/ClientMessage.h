#pragma once

#include <vector>
#include <memory>

#include "MessageType.h"
#include "Client.h"

#include "../injectable/wow/game/WowVector3f.h"

class ClientMessage {
public:
	ClientMessage(std::shared_ptr<Client> client) :
	cl(client),
		waypoints(nullptr),
		subscriptions(nullptr)
	{

	}
	std::shared_ptr<Client> cl;
	MessageType type;
	char eject;
	const std::vector<WowVector3f>* waypoints;
	const std::vector<std::string>* subscriptions;
};
