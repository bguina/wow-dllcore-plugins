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
	mContext(nullptr),
	mWaypoints(nullptr)
{
}

ABen::~ABen() {
}

bool ABen::attach(std::shared_ptr<WowGame> game) {
	IBenGameInterpretation* newInterpretation(mInterpret->interpret(*game));

	if (nullptr != newInterpretation) {
		mContext.reset(newInterpretation);
		return BaseWowBot::attach(game);
	}
	
	return false;
}

void ABen::onResume() {
}
void ABen::onEvaluate() {
}
void ABen::onPause() {
}

void ABen::onGamePlayStarted() {

}

void ABen::onGameplayStopped() {

}

void ABen::onTargetChanged() {

}

void ABen::onCombatStart() {

}

void ABen::onCombatRelief() {

}
void ABen::onCombatEnd() {

}

bool ABen::onEvaluatedIdle() {
	return false;
}

bool ABen::handleWowMessage(ServerWowMessage& cl) {
	bool handled = false;
	mDbg << FileLogger::verbose << "handleServerMessage " << (int)cl.type << FileLogger::normal << std::endl;

	switch (cl.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
		mWaypoints = std::make_unique<LinearPathFinder>(*cl.waypoints);
		mDbg << FileLogger::verbose << "Loaded pathfinder with " << cl.waypoints->size() << "waypoints! thanks! " << (int)cl.type << FileLogger::normal << std::endl;
		handled = true;
	default:
		break;
	}

	return handled;
}
