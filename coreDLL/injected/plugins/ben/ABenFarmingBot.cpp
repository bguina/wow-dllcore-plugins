#include "pch.h"

#include "ABenFarmingBot.h"

#include "../../process/wow/object/WowActivePlayerObject.h"

#include "../../../pathfinder/LinearPathFinder.h"

const std::string TAG = "BenPlayerClassBot";

ABenFarmingBot::ABenFarmingBot(WowGame& game, const std::string& tag) :
	ABenBot(game, tag)
{
	mDbg << FileLogger::warn << "ABenFarming("<< tag << ")" << FileLogger::normal << FileLogger::normal << std::endl;
}

ABenFarmingBot::~ABenFarmingBot() {
	mDbg << FileLogger::warn << "~ABenFarmingBot" << FileLogger::normal << std::endl;
}

bool ABenFarmingBot::handleServerMessage(const PluginServerMessage& serverMessage) {

	if (!AWowBot::handleServerMessage(serverMessage)) {
		switch (serverMessage.type)
		{
		case MessageType::WAYPOINTS:
			mDbg << FileLogger::info << " loading LinearPathFinder with " << serverMessage.data.waypoints->size() << " positions" << FileLogger::normal << std::endl;
			mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.data.waypoints);
			return true;
		default:
			break;
		}
	}

	return false;
}
