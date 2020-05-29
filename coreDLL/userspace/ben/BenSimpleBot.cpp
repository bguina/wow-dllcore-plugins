#include "pch.h"

#include "../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../injectable/wow/game/spell/ISpell.h"

#include "BenSimpleBot.h"

const std::string TAG = "BenSimpleBot";

BenSimpleBot::BenSimpleBot(WowGame& game) :
	ABenBot(game, TAG)
{
}

BenSimpleBot::~BenSimpleBot()
{
}

void BenSimpleBot::onEvaluate() {

	std::list<std::shared_ptr<WowUnitObject>> allObjects = mGame.getObjectManager().allOfType<WowUnitObject>();





	int spellId = 168;
	auto self = mGame.getObjectManager().getActivePlayer();
	mDbg << self << std::endl;
	for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
		auto obj = *it;

		mDbg << obj->getTypeLabel() << " @" << obj->getPosition() << std::endl;
	}

	if (mGame.traceLine(self->getPosition(), self->getPosition(), 0)) {
		mDbg << "I can't see to my ... " << std::endl;

	}
	mDbg.flush();
}
