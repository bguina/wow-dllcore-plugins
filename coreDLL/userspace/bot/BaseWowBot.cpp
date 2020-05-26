#include "pch.h"

#include <vector>

#include "BaseWowBot.h"

#include "../../logger/FileLogger.h"

#include "../../injectable/wow/game/WowGame.h"
#include "../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../injectable/wow/pathfinder/LinearPathFinder.h"

BaseWowBot::BaseWowBot(WowGame& game, const std::string& tag) :
	mGame(game),
	mDbg(tag)
{
}

BaseWowBot::~BaseWowBot()
{
}

const char* BaseWowBot::getTag() const {
	return mDbg.getTag().c_str();
}

void BaseWowBot::_logDebug() const {
	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileLogger::info;

	if (nullptr != self) {
		// show info relative to self
		mDbg << "Self position is " << self->getPosition() << " angle is " << self->getFacingDegrees() << std::endl;
		mDbg << "Self in combat: " << self->isInCombat() << std::endl;
		mDbg << FileLogger::info << "Spellbook Count " << mGame.getSpellBook().getSpellBookCount() << FileLogger::normal << std::endl;
		auto targetGuid = self->getTargetGuid();

		if (0 != targetGuid) {

			auto target = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
			if (nullptr != target) {
				mDbg << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << self->canAttack(mGame, *target) << std::endl;
			}
			else {
				mDbg << FileLogger::err << "Target: " << targetGuid.upper() << targetGuid.lower() << " can be attacked? " << (uint32_t)self->canAttack(mGame, *target) << FileLogger::info << std::endl;
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

