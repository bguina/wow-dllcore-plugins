#pragma once

#include "../ABenBot.h"
#include "../../PluginServerMessage.h"

/*

	Base class for a farming rotation over a long period.

	Implements pathFinder to abstracts a farming session lifecycle.
		- _onFarmableRegionReached
		- _onFarmableRegionLeft
*/
#include "Farm.h"

class ABenFarmingBot : public ABenBot
{
public:
	ABenFarmingBot(WowGame& game, const std::string& tag);
	virtual ~ABenFarmingBot();

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) override;

protected:
	bool _getFarmInSight(Farm& targetFarm);

	std::unique_ptr<IPathFinder> mPathFinder;
};
