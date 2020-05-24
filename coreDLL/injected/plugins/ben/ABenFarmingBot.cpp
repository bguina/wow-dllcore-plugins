#include "pch.h"

#include "ABenFarmingBot.h"

#include "../../process/wow/object/WowActivePlayerObject.h"

#include "../../../pathfinder/LinearPathFinder.h"

const std::string TAG = "BenPlayerClassBot";

ABenFarmingBot::ABenFarmingBot(WowGame& game, const std::string& tag) :
	ABenBot(game, tag)
{
	mDbg << FileDebugger::warn << "ABenFarming("<< tag << ")" << FileDebugger::normal << FileDebugger::normal << std::endl;
}

ABenFarmingBot::~ABenFarmingBot() {
	mDbg << FileDebugger::warn << "~ABenFarmingBot" << FileDebugger::normal << std::endl;
}

bool ABenFarmingBot::handleServerMessage(const PluginServerMessage& serverMessage) {

	if (!AWowBot::handleServerMessage(serverMessage)) {
		switch (serverMessage.type)
		{
		case MessageType::WAYPOINTS:
			mDbg << FileDebugger::info << " loading LinearPathFinder with " << serverMessage.data.waypoints->size() << " positions" << FileDebugger::normal << std::endl;
			mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.data.waypoints);
			return true;
		default:
			break;
		}
	}

	return false;
}
