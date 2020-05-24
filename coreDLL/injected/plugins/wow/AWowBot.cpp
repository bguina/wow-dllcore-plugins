#include "pch.h"

#include <vector>

#include "AWowBot.h"

#include "../../../pathfinder/LinearPathFinder.h"
#include "../../../logger/FileLogger.h"

#include "../../process/wow/WowGame.h"
#include "../../process/wow/object/WowActivePlayerObject.h"

AWowBot::AWowBot(WowGame& game, const std::string& tag) :
	mGame(game),
	mDbg(tag)
{
}

AWowBot::~AWowBot()
{
}

bool AWowBot::pause(bool paused) {
	if (paused)
		mGame.getWindowController()->releaseAllKeys();

	return APausablePlugin::pause(paused);
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