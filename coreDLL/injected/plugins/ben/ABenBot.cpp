#include "pch.h"

#include <vector>

#include "ABenBot.h"

#include "../../pathfinder/LinearPathFinder.h"
#include "../../logger/FileLogger.h"

#include "../../process/wow/WowGame.h"
#include "../../process/wow/object/WowActivePlayerObject.h"

const std::string TAG = "ABenBot";

ABenBot::ABenBot(WowGame& game, const std::string& tag) :
	AWowBot(game, tag),
	mPathFinder(nullptr)
{
}

ABenBot::~ABenBot() {
}

bool ABenBot::pause(bool paused) {
	if (AWowBot::pause(paused)) {
		if (isPaused()) _onPaused();
		else _onResumed();
		return true;
	}
	return false;
}

void ABenBot::_onResumed() {
	mDbg << FileLogger::warn << TAG << " resumed" << FileLogger::normal << std::endl;
}

void ABenBot::_onPaused() {
	mDbg << FileLogger::warn << TAG << " paused" << FileLogger::normal << std::endl;
}

void ABenBot::_logDebug() const {
	AWowBot::_logDebug();

	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());

	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << FileLogger::info << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << FileLogger::warn << " without PathFinder" << std::endl;

	mDbg << FileLogger::normal;
}
