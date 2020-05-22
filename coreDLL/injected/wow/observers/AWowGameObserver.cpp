#include "pch.h"

#include "AWowGameObserver.h"
#include "../WowGame.h"

AWowGameObserver::AWowGameObserver(const std::string& tag, Client& client, unsigned long periodMs) :
	AGameObserver<WowGame>(tag, periodMs),
	mClient(client)
{

}
