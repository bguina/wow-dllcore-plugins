#include "pch.h"

#include "AWowGameObserver.h"
#include "../WowGame.h"

AWowGameObserver::AWowGameObserver(ServerSDK& server, unsigned long periodMs) : AGameObserver<WowGame>(periodMs), mServer(server)
{}
