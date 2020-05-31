#include <vector>

#include "ABen.h"

#include "game/WowGame.h"
#include "game/object/WowGuid128.h"
#include "game/object/WowActivePlayerObject.h"
#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"

const std::string TAG = "ABen";

ABen::ABen(const std::string& tag) :
	BaseWowBot(tag),
	mPathFinder(nullptr)
{
}

ABen::~ABen() {
}

void ABen::onResume(WowGame& game) {

}

void ABen::onPause(WowGame& game) {

}

void ABen::onEvaluate(WowGame& game) {

}

bool ABen::handleWowMessage(ServerWowMessage& cl) {
	bool handled = false;
	mDbg << FileLogger::verbose << "handleServerMessage " << (int)cl.type << FileLogger::normal << std::endl;

	switch (cl.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
		mPathFinder = std::make_unique<LinearPathFinder>(*cl.waypoints);
		mDbg << FileLogger::verbose << "Loaded pathfinder with " << cl.waypoints->size() << "waypoints! thanks! " << (int)cl.type << FileLogger::normal << std::endl;
		handled= true;
	default:
		break;
	}

	return handled;
}

void ABen::_logDebug(const WowGame& game) const {

	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());

	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
	}
}
