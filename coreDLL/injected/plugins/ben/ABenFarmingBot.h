#pragma once

#include "ABenBot.h"
#include "../PluginServerMessage.h"

/*
	Base class for a farming rotation over a long period. 
*/
class ABenFarmingBot : public ABenBot
{
public:
	ABenFarmingBot(WowGame& game, const std::string& tag);
	virtual ~ABenFarmingBot();

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) override;

protected:
	std::unique_ptr<IPathFinder> mPathFinder;
};
