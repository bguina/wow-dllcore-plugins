#pragma once

#include "Client.h"

#include "../../observers/AGameObserver.h"

class Client;
class WowGame;

class AWowGameObserver : public AGameObserver<WowGame> {
protected:
	AWowGameObserver(const std::string& tag, Client& client, unsigned long periodMs);

protected:
	Client& mClient;
};
