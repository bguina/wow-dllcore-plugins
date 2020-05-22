#include "pch.h"

#include "AWowGameObserver.h"
#include "../WowGame.h"

AWowGameObserver::AWowGameObserver(const std::string& tag, ServerSDK& server, unsigned long periodMs) : 
	AGameObserver<WowGame>(tag, periodMs),
	mServer(server)
{}
