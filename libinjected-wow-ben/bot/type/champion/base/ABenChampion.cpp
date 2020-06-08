#include "ABenChampion.h"

#include <set>

#include "../../../gameplay/snapshot/object/WowUnitSnapshot.h"

ABenChampion::ABenChampion(ABenWowGameEvaluator* gameplay, const std::string& tag) :
	ABenChampion(gameplay, tag, nullptr)
{
}

ABenChampion::ABenChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenAgent(gameplay, tag),
	mRunagate(runagate)
{
}

ABenChampion::~ABenChampion() = default;

bool ABenChampion::onEvaluatedIdle()
{

	return onEvaluatedInFight();
}

bool ABenChampion::runAway() {
	return nullptr != mRunagate && mRunagate->runAway();
}

void ABenChampion::onUnitTap(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
}
