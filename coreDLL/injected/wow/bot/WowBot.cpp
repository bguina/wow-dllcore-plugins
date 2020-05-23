#include "pch.h"

#include <vector>

#include "WowBot.h"

#include "../../../pathfinder/LinearPathFinder.h"
#include "../../../debugger/FileDebugger.h"
#include "../WowGame.h"
#include "../objectmanager/WowActivePlayerObject.h"

WowBot::WowBot(WowGame& game, const std::string& tag) :
	mGame(game),
	mDbg(tag)
{
}

WowBot::~WowBot()
{
}

void WowBot::pause(bool paused) {
	if (paused)
		mGame.getWindowController()->releaseAllKeys();

	Bot::pause(paused);


}

bool WowBot::isPaused() const {
	return mPaused;
}

void WowBot::logDebug() const {
	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileDebugger::info;

	if (nullptr != self) {
		// show info relative to self
		mDbg << "Self position is " << self->getPosition() << " angle is " << self->getFacingDegrees() << " target unit is " << self->getTargetGuid().upper() << std::endl;
		mDbg << "Self in combat: " << self->isInCombat() << std::endl;



		// show info relative to a random unit around, if any
		std::shared_ptr<WowUnitObject> any = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObjectType::Unit);
		if (nullptr != any) {
			mDbg << "anyunit position is " << any->getPosition()
				<< " facing it requires angle of " << self->getFacingDegreesTo(*any)
				<< " angle delta is " << self->getFacingDeltaDegrees(*any) << std::endl;
		}
	}
	mDbg << FileDebugger::normal;
}

