#include "ABenChampion.h"

#include "../../../gameplay/snapshot/object/WowUnitSnapshot.h"

ABenChampion::ABenChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag) :
	ABenAgent(gameplay, tag),
	mRunagate(nullptr)
{
}

ABenChampion::~ABenChampion() = default;

bool ABenChampion::onEvaluatedIdle()
{
	return onEvaluatedAsChampion();
}

bool ABenChampion::runAway() {
	return nullptr != mRunagate && mRunagate->runAway();
}

bool ABenChampion::isInCombat() const
{
	return nullptr != mSelf && mSelf->isInCombat();
}

void ABenChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
	mCombatUnits.remove(object);
	// todo loot it?
}

void ABenChampion::onUnitAggro(const std::shared_ptr<const WowUnitObject> & object)
{
	FileLogger dbg(mLog, "ABenChampion:onUnitAggro");

	dbg << dbg.e() << "adding unit of level " << object->getLevel() << dbg.endl();
	if (mCombatUnits.end() == std::find(mCombatUnits.begin(), mCombatUnits.end(), object)) 
		mCombatUnits.push_back(object);
}

void ABenChampion::onUnitAggroLost(const std::shared_ptr<const WowUnitObject> & object)
{
	FileLogger dbg(mLog, "ABenChampion:onUnitAggroLost");

	dbg << dbg.e() << "removing unit of level " << object->getLevel() << dbg.endl();
	mCombatUnits.remove(object);
}

void ABenChampion::onUnitTap(const std::shared_ptr<const WowUnitObject> & object)
{
}
