#include "pch.h"

#include <vector>

#include "AWowBot.h"

#include "../../../pathfinder/LinearPathFinder.h"
#include "../../../logger/FileLogger.h"

#include "../../process/wow/WowGame.h"
#include "../../process/wow/object/WowActivePlayerObject.h"

AWowBot::AWowBot(WowGame& game, const std::string& tag) :
	APausablePlugin(tag),
	mGame(game)
{
}

AWowBot::~AWowBot()
{
}

bool AWowBot::pause(bool paused) {
	if (APausablePlugin::pause(paused)) {
		if (isPaused()) _onPaused();
		else _onResumed();
		return true;
	}
	return false;
}

bool AWowBot::isPaused() const {
	return mPaused;
}

void AWowBot::onD3dRender() {
	if (!isPaused()) {
		_onRunning();
	}

	mDbg.flush();
}

void AWowBot::_onResumed() {
	//mDbg << FileLogger::warn << mDbg.getTag() << " resumed" << FileLogger::normal << std::endl;
}

void AWowBot::_onPaused() {
	//mGame.getWindowController()->releaseAllKeys();
	//mDbg << FileLogger::warn << mDbg.getTag() << " paused" << FileLogger::normal << std::endl;
}

void AWowBot::_logDebug() const {
	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileLogger::info;

	if (nullptr != self) {
		// show info relative to self
		mDbg << "Self position is " << self->getPosition() << " angle is " << self->getFacingDegrees() << std::endl;
		mDbg << "Self in combat: " << self->isInCombat() << std::endl;
		auto targetGuid = self->getTargetGuid();

		if (0 != targetGuid) {

			auto target = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
			if (nullptr != target) {
				mDbg << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << self->canAttack(mGame, *target) << std::endl;
			}
			else {
				mDbg << FileLogger::err << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << self->canAttack(mGame, *target) << FileLogger::info << std::endl;
			}

		}

		// show info relative to a random unit around, if any
		std::shared_ptr<WowUnitObject> any = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObjectType::Unit);
		if (nullptr != any) {
			mDbg << "anyunit position is " << any->getPosition()
				<< " facing it requires angle of " << self->getFacingDegreesTo(*any)
				<< " angle delta is " << self->getFacingDeltaDegrees(*any) << std::endl;
		}
	}
	mDbg << FileLogger::normal;
}