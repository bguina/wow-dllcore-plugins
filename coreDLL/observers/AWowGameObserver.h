#pragma once

#include "ServerSDK.h"

#include "AGameObserver.h"

class ServerSDK;
class WowGame;

class AWowGameObserver : public AGameObserver<WowGame> {
protected:
	AWowGameObserver(const std::string& tag, ServerSDK& server, unsigned long periodMs);

protected:
	ServerSDK& mServer;
};
