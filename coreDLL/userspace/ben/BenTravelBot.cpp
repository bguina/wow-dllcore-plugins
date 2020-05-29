#include "pch.h"

#include <bitset>

#include "../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../injectable/wow/game/spell/ISpell.h"

#include "BenTravelBot.h"

const std::string TAG = "BenTravelBot";

BenTravelBot::BenTravelBot(WowGame& game) :
	ABenBot(game, TAG)
{
}

BenTravelBot::~BenTravelBot()
{
}

void BenTravelBot::onResume() {
	int spellId = 168;
	auto self = mGame.getObjectManager().getActivePlayer();

	if (nullptr != self) {
		const WowGuid128 targetGuid = self->getTargetGuid();
		auto target = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
		if (nullptr == target) {
			mDbg << FileLogger::err << "could not find own target in objmgr!" << FileLogger::normal << std::endl;
			return;
		}


		auto pSelfGuid = self->getGuidPtr();
		mGame.getSpellBook().castSpell(mGame, spellId, pSelfGuid);
		mDbg << FileLogger::info << "casted spell " << spellId << ": " << false << FileLogger::normal << std::endl;
	}
	mDbg.flush();
}

void BenTravelBot::onPause() {

}

void BenTravelBot::onEvaluate() {
	std::list<std::shared_ptr<WowUnitObject>> allObjects = mGame.getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit);
	
	Fireball fireball = Fireball();
	fireball.getRankId(1);

	//_logDebug();

	for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
		mDbg << FileLogger::info << "found unit " << (*it)->getTypeLabel() << " guid decimal " << (*it)->getGuid().upper() << (*it)->getGuid().lower() << FileLogger::normal << std::endl;
	}

	mLua.run("OpenAllBags();");

	mDbg.flush();

}

