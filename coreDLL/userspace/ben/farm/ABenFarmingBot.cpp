#include "pch.h"

#include "ABenFarmingBot.h"

#include "../../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../../injectable/wow/pathfinder/LinearPathFinder.h"

const std::string TAG = "BenPlayerClassBot";

ABenFarmingBot::ABenFarmingBot(WowGame& game, const std::string& tag) :
	ABenBot(game, tag),
	mPathFinder(nullptr)
{
	//mDbg << FileLogger::warn << "ABenFarming(" << tag << ")" << FileLogger::normal << FileLogger::normal << std::endl;
}

ABenFarmingBot::~ABenFarmingBot() {
	//mDbg << FileLogger::warn << "~ABenFarmingBot" << FileLogger::normal << std::endl;
}

//bool ABenFarmingBot::handleServerMessage(const PluginServerMessage& serverMessage) {
//	switch (serverMessage.type)
//	{
//	case MessageType::POST_DLL_DATA_3DPATH:
//		mDbg << FileLogger::info << " loading LinearPathFinder with " << serverMessage.waypoints->size() << " positions" << FileLogger::normal << std::endl;
//		mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.waypoints);
//		return true;
//	default:
//		break;
//	}
//
//	return AWowBot::handleServerMessage(serverMessage);
//}
