#include "pch.h"

#include <vector>

#include "ABenBot.h"

#include "../../pathfinder/LinearPathFinder.h"
#include "../../debugger/FileDebugger.h"

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
	mDbg << FileDebugger::warn << TAG << " resumed" << FileDebugger::normal << std::endl;
}

void ABenBot::_onPaused() {
	mDbg << FileDebugger::warn << TAG << " paused" << FileDebugger::normal << std::endl;
}

void ABenBot::_logDebug() const {
	AWowBot::_logDebug();

	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());

	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << FileDebugger::info << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << FileDebugger::warn << " without PathFinder" << std::endl;

	mDbg << FileDebugger::normal;
}
