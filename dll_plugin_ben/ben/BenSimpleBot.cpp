
#include "BenSimpleBot.h"

#include "game/object/WowActivePlayerObject.h"
#include "game/spell/ISpell.h"

const std::string TAG = "BenSimpleBot";

BenSimpleBot::BenSimpleBot() :
	ABenBot(TAG)
{
}

BenSimpleBot::~BenSimpleBot()
{
}

void BenSimpleBot::onEvaluate(WowGame& game) {

	std::list<std::shared_ptr<WowUnitObject>> allObjects = game.getObjectManager().allOfType<WowUnitObject>();



	int spellId = 168;
	auto self = game.getObjectManager().getActivePlayer();
	mDbg << self << std::endl;
	for (auto it = allObjects.begin(); it != allObjects.end(); ++it) {
		auto obj = *it;

		mDbg << obj->getTypeLabel() << " @" << obj->getPosition() << std::endl;
	}

	if (game.traceLine(self->getPosition(), self->getPosition(), 0)) {
		mDbg << "I can't see to my ... " << std::endl;

	}
}
