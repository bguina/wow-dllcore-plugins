#include "pch.h"

#include <bitset>

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
	ABenBot::onResume();

	int spellId = 168;
	auto self = mGame.getObjectManager().getActivePlayer();
	const WowGuid128 targetGuid = self->getTargetGuid();
	auto target = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(targetGuid);
	if (nullptr == target) {
		mDbg << FileLogger::err << "could not find own target in objmgr!" << FileLogger::normal << std::endl;
		return;
	}

	const SpellbookDescriptor* spellEntry = mGame.getSpellBook().getSpell(spellId);

	if (nullptr == spellEntry) {
		mDbg << FileLogger::err << "could not find a spellEntry with spellId " << spellId << FileLogger::normal << std::endl;

		auto allSpells = mGame.getSpellBook().listSpells();
		for (auto it = allSpells.begin(); it != allSpells.end(); ++it) {
			mDbg << FileLogger::warn << "try this spell instead: " << it->id << ": " << std::bitset<32>(it->flags) << FileLogger::normal << std::endl;
		}

		return;
	}
	mDbg << FileLogger::info << "found spellEntry of spell " << spellId << ": " << std::bitset<32>(spellEntry->flags) << FileLogger::normal << std::endl;

	auto pSelfGuid = self->getGuidPtr();
	mGame.getSpellBook().castSpell(mGame, spellEntry->id, pSelfGuid);
	mDbg << FileLogger::info << "casted spell " << spellId << ": " << false << FileLogger::normal << std::endl;
}



void BenTravelBot::onEvaluate() {
	std::list<std::shared_ptr<WowUnitObject>> allObjects = mGame.getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit);

	//_logDebug();

	for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
		mDbg << FileLogger::info << "found unit " << (*it)->getTypeLabel() << " guid decimal " << (*it)->getGuid().upper() << (*it)->getGuid().lower() << FileLogger::normal << std::endl;
	}


}

void BenTravelBot::onPause() {
	ABenBot::onPause();

}