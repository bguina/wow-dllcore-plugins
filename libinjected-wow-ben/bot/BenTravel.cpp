#include <bitset>

#include "game/object/WowActivePlayerObject.h"
//#include "game/spell/ISpell.h"

#include "BenTravel.h"

const std::string TAG = "BenTravel";

BenTravel::BenTravel() :
	ABen(TAG)
{
}

BenTravel::~BenTravel()
{
}

void BenTravel::onResume(WowGame& game) {
	int spellId = 168;
	auto self = game.getObjectManager().getActivePlayer();

	if (nullptr != self) {
		const WowGuid128 targetGuid = self->getTargetGuid();
		auto target = game.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
		if (nullptr == target) {
			mDbg << FileLogger::err << "could not find own target in objmgr!" << FileLogger::normal << std::endl;
			return;
		}


		auto pSelfGuid = self->getGuidPtr();
		game.getSpellBook().castSpell(game, spellId, pSelfGuid);
		mDbg << FileLogger::info << "casted spell " << spellId << ": " << false << FileLogger::normal << std::endl;
	}
}

void BenTravel::onPause(WowGame& game) {

}

void BenTravel::onEvaluate(WowGame& game) {
	std::list<std::shared_ptr<WowUnitObject>> allObjects = game.getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit);
	
	//Fireball fireball = Fireball();
	//fireball.getRankId(1);

	//_logDebug();

	for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
		mDbg << FileLogger::info << "found unit " << (*it)->getTypeLabel() << " guid decimal " << (*it)->getGuid().upper() << (*it)->getGuid().lower() << FileLogger::normal << std::endl;
	}

	Lua(game).run("OpenAllBags();");
	//Fireball1 f;
}
