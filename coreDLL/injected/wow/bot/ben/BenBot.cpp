#include "pch.h"

#include <vector>

#include "BenBot.h"
#include "../../WowGame.h"

#include "../../../../pathfinder/LinearPathFinder.h"
#include "../../../../debugger/FileDebugger.h"
#include "../../objectmanager/WowActivePlayerObject.h"

const std::string TAG = "BenBot";

BenBot::BenBot(WowGame& game, const std::string& tag) :
	WowBot(game, tag),
	mPathFinder(nullptr)
{
}

BenBot::~BenBot()
{
}

void BenBot::pause(bool paused) {
	WowBot::pause(paused);

	mDbg << FileDebugger::info << TAG << " " << (mPaused ? "paused" : "running") << FileDebugger::normal << std::endl;
}

void BenBot::logDebug() const {
	WowBot::logDebug();

	// show info relative otof current pathfinder
	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << " without PathFinder" << std::endl;
}

void BenBot::run() {
	mDbg << "\n\n";
	mDbg << FileDebugger::info << TAG << "::run()" << FileDebugger::normal << std::endl;

	logDebug();

}

void BenBot::loadPathWaypoints(const std::vector<Vector3f>& waypoints) {
	mPathFinder = std::make_unique<LinearPathFinder>(waypoints);
}
