#include <vector>

#include "BaseWowBot.h"

#include "../game/WowGame.h"
#include "../game/object/WowActivePlayerObject.h"
#include "../pathfinder/LinearPathFinder.h"

BaseWowBot::BaseWowBot(const std::string& tag) :
	mGame(nullptr),
	mDbg(tag)
{
}

BaseWowBot::~BaseWowBot() {
}

bool BaseWowBot::attach(std::shared_ptr<WowGame> game) {
	mGame = game;
	return true;
}

const std::string& BaseWowBot::getTag() const {
	return mDbg.getTag();
}

void BaseWowBot::_logDebug() const {
	std::shared_ptr<const WowActivePlayerObject> self = mGame->getObjectManager().getActivePlayer();
	mDbg << FileLogger::info;

	if (nullptr != self) {
		// show info relative to self
		mDbg << "Self position is " << self->getPosition() << " angle is " << self->getFacingDegrees() << std::endl;
		mDbg << "Self in combat: " << self->isInCombat() << std::endl;
		auto targetGuid = self->getTargetGuid();

		if (0 != targetGuid) {

			std::shared_ptr<const WowUnitObject> target = mGame->getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
			if (nullptr != target) {
				mDbg << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << self->canAttack(*mGame, *target) << std::endl;
			}
			else {
				mDbg << FileLogger::err << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << (uint32_t)self->canAttack(*mGame, *target) << FileLogger::info << std::endl;
			}
		}

		// show info relative to a random unit around, if any
		std::shared_ptr<const WowUnitObject> any = mGame->getObjectManager().anyOfType<WowUnitObject>(WowObjectType::Unit);
		if (nullptr != any) {
			mDbg << "anyunit position is " << any->getPosition()
				<< " facing it requires angle of " << self->getFacingDegreesTo(*any)
				<< " angle delta is " << self->getFacingDeltaDegrees(*any) << std::endl;
		}
	}
	mDbg << FileLogger::normal;
}

