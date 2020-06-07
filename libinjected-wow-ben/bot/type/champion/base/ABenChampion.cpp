#include "ABenChampion.h"

#include <set>

#include "../../../gameplay/snapshot/object/WowUnitSnapshot.h"

ABenChampion::ABenChampion(IBenGameplay* gameplay, const std::string& tag) :
	ABenChampion(gameplay, tag, nullptr)
{
}

ABenChampion::ABenChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenAgent(gameplay, tag),
	mRunagate(runagate)
{
}

ABenChampion::~ABenChampion() = default;

bool ABenChampion::onEvaluatedIdle()
{

	return onEvaluatedInFight();
}

bool ABenChampion::updateFromSnapshot(const std::shared_ptr<const IBenGameSnapshot> & snapshot)
{
	ABenAgent::updateFromSnapshot(snapshot);

	FileLogger dbg(mDbg, "updateFromSnapshot");
	const auto& objMgr(mGame->getObjectManager());
	auto prevEnemies = snapshot->getHostileList();
	auto enemies = objMgr.allOfType<WowUnitObject>();

	//enemies.remove_if([&](const std::shared_ptr<const WowUnitObject>& v) { return !mSelf->canAttack(*mGame, *v); });

	{
		// trigger onUnitDeath
		std::set<WowGuid128> enemiesKilled;

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			const auto& enemy(*it);

			if (enemy->isDead())
			{
				auto oldVersion(snapshot->getUnitByGuid(enemy->getGuid()));

				if (nullptr == oldVersion || !oldVersion->isDead())
				{
					onUnitDeath(enemy);
				}
				//else
			//	dbg << dbg.w() << " target previously already dead? " << oldVersion << dbg.endl();

			}
		}
	}

	return true;
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
