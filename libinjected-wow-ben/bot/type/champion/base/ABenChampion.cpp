#include "ABenChampion.h"

#include <algorithm>
#include <set>

#include "../../../gameplay/snapshot/object/WowUnitSnapshot.h"

ABenChampion::ABenChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenAgent(gameplay, tag),
	mRunagate(runagate)
{

}

ABenChampion::~ABenChampion() = default;

bool ABenChampion::onEvaluatedIdle() {
	FileLogger dbg(mDbg, "onEvaluatedIdle");
	const auto& objMgr(mGame->getObjectManager());
	const auto snapshot(mGameplay->getLastFrame());
	auto enemies = objMgr.allOfType<WowUnitObject>();

	// test lookup frame 5 seconds ago
	{
		//const auto oldSnapshot(mGameplay->getFrameAtTimestamp(-4000));

		//dbg << dbg.e() << "oldSnapshot=" << oldSnapshot << dbg.endl();
	}

	enemies.remove_if([&](const std::shared_ptr<const WowUnitObject>& v) { return !mSelf->canAttack(*mGame, *v); });

	{
		IBenGameSnapshot::UnitList previouslyAggroedEnemies;

		std::copy_if(snapshot->listHostiles().begin(), snapshot->listHostiles().end(),
			std::back_inserter(previouslyAggroedEnemies),
			[&](const std::shared_ptr<WowUnitSnapshot>& v) { return v->getTargetGuid() == mSelf->getGuid(); });


		// trigger onUnitUnaggro
		for (auto it = previouslyAggroedEnemies.begin(); it != previouslyAggroedEnemies.end(); ++it) {
			const auto unit = objMgr.getObjectByGuid<WowUnitObject>((*it)->getGuid());

			if (nullptr == unit || unit->vanished() || unit->getTargetGuid() != mSelf->getGuid())
			{
				onUnitUnaggro(*unit);
			}
		}

		// trigger onUnitAggro
		for (auto it = enemies.begin(); it != enemies.end(); ++it) {
			if ((*it)->getTargetGuid() == mSelf->getGuid())
			{
				onUnitAggro(**it);
				mTargetingMe.insert((*it)->getGuid());
			}
		}
	}

	{
		// trigger onUnitTap / onUnitUntap
		//todo
	}

	{
		// trigger onUnitKill
		std::set<WowGuid128> enemiesKilled;
		IBenGameSnapshot::UnitList oldEnemies(snapshot->listHostiles());

		for (auto it = enemies.begin(); it != enemies.end(); ++it) {
			if ((*it)->isDead())
			{
				const auto& deadEnemy(**it);
				bool wasAlive(std::find_if(oldEnemies.begin(), oldEnemies.end(),
					[&deadEnemy](const std::shared_ptr<WowUnitSnapshot>& v) { return deadEnemy.getGuid() == v->getGuid() && !v->isDead(); }) != oldEnemies.end());

				if (wasAlive)
				{
					onUnitKill(deadEnemy);
				}
			}
		}
	}

	return onEvaluatedInFight();
}
bool ABenChampion::runAway() {
	return nullptr != mRunagate && mRunagate->runAway();
}

void ABenChampion::onUnitAggro(const WowUnitObject & object)
{
}

void ABenChampion::onUnitUnaggro(const WowUnitObject & object)
{
}

void ABenChampion::onUnitTap(const WowUnitObject & object)
{
}

void ABenChampion::onUnitKill(const WowUnitObject & object)
{
}
