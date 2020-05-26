#include "pch.h"

#include "AWowGameObserver.h"
#include "../game/WowGame.h"

AWowGameObserver::AWowGameObserver(const std::string& tag, Client& client, unsigned long periodMs) :
	ARecurrentServerObserver<WowGame>(tag, periodMs),
	mClient(client)
{

}
