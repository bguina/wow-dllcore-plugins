#include "pch.h"

#include <vector>

#include "ABenBot.h"

#include "../../injectable/wow/game/WowGame.h"
#include "../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../injectable/wow/pathfinder/LinearPathFinder.h"

const std::string TAG = "ABenBot";

ABenBot::ABenBot(WowGame& game, const std::string& tag) :
	BaseWowBot(game, tag),
	mPathFinder(nullptr)
{
}

ABenBot::~ABenBot() {
}

void ABenBot::onResume() {

}

void ABenBot::onEvaluate() {

}

void ABenBot::onPause() {

}

bool ABenBot::handleServerMessage(ClientMessage& serverMessage) {
	bool handled = false;
	mDbg << FileLogger::verbose << "handleServerMessage " << (int)serverMessage.type << FileLogger::normal << std::endl;

	switch (serverMessage.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
		mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.waypoints);
		mDbg << FileLogger::verbose << "Loaded pathfinder with " << serverMessage.waypoints->size() << "waypoints! thanks! " << (int)serverMessage.type << FileLogger::normal << std::endl;
		handled= true;
	default:
		break;
	}

	mDbg.flush();
	return handled;
}

void ABenBot::_logDebug() const {

	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());

	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		//mDbg << FileLogger::info << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	//else mDbg << FileLogger::warn << " without PathFinder" << std::endl;

	//mDbg << FileLogger::normal;
}