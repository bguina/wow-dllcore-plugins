#include "ABenPatroller.h"

#include <algorithm>

ABenPatroller::ABenPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenChampion* combatBot) :
	ABenPatroller(gameplay, tag, nullptr, combatBot)
{
}

ABenPatroller::ABenPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot) :
	ABenAgent(gameplay, tag),
	mTraveler(travelBot),
	mChamp(combatBot)
{
	if (nullptr != travelBot) mNestedAgents.push_back(travelBot);
	if (nullptr != combatBot) mNestedAgents.push_back(combatBot);
}

ABenPatroller::~ABenPatroller() = default;

void ABenPatroller::setCombatBot(ABenChampion* agent)
{
	// todo pause previous agent?
	mChamp.reset(agent);
}

bool ABenPatroller::onEvaluatedIdle()
{
	FileLogger dbg(mLog, "ABenPatroller:onEvaluatedIdle");

	if (nullptr == mChamp)
	{
		dbg << dbg.e() << "no champ !" << dbg.endl();
		return false;
	}

	if (mChamp->onEvaluate())
	{
		dbg << dbg.i() << "champ fighting " << dbg.endl();
		return true;
	}

	if (nullptr == mSelf)
	{
		dbg << dbg.w() << "nullptr == mSelf" << dbg.endl();
		return false;
	}

	// no combat to handle
	const auto patrolAttackPredicate = [&](const std::shared_ptr<WowUnitObject>& v)->bool
	{
		//Vector3f dToPath;
		return !mSelf->isFriendly(*v) && // not friendly
			v->getLevel() > mSelf->getLevel() - 6 && // not a grey unit
			!v->isDead() //&& !v->isTappedByOthers() // not decayed
		;
	};

	{
		Vector3f nextWaypoint;

		std::list<std::shared_ptr<WowUnitObject>> hostiles(mGame->getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit));

		// exclude out or range units
		std::list<std::shared_ptr<WowUnitObject>> enemiesInRange;

		std::copy_if(hostiles.begin(), hostiles.end(), std::back_inserter(enemiesInRange), patrolAttackPredicate);

		enemiesInRange.sort([&](const auto& a, const auto& b)->bool {
			return evaluatePatrolRelativeThreat(nextWaypoint, *a) < evaluatePatrolRelativeThreat(nextWaypoint, *b);
		});

		int i = 0;
		for (auto it = enemiesInRange.begin(); i < 5 && it != enemiesInRange.end(); ++it)
		{
			++i;
			dbg << dbg.i() << "threat #" << i << ": " << (*it)->getPosition().getDistanceTo(mSelf->getPosition()) << " level " << (*it)->getLevel() << "  " << evaluatePatrolRelativeThreat(nextWaypoint, **it) << dbg.endl();
		}
		
		if (!enemiesInRange.empty())
		{
			const auto& kill(*enemiesInRange.begin());
			
			mGame->getWindowController()->releaseAllKeys();
			dbg << dbg.i() << "pullCombat " << kill->getGuid() << dbg.endl();
			mChamp->pullCombat(kill);
		}
		else
		{
			if (getPathFinder().getNextPosition(mSelf->getPosition(), nextWaypoint))
			{
				//mGameplay->estimatePathThreat(mGame, position, time)
				// list all mobs around the path, listed by 1/(distance to us * nearest distance to path segment)
				//std::vector<std::shared_ptr<WowUnitSnapshot>> hostiles(mGameplay->front()->getHostileList().begin(), mGameplay->front()->getHostileList().end());

				// todo avoid dangerous positions / ground holes

				dbg << dbg.e() << "moveTo " << dbg.endl();
				mSelf->moveTo(*mGame, nextWaypoint);
			}
			else
			{
				dbg << dbg.e() << "getNextPosition() failed" << dbg.endl();
				return false;
			}
		}
	}

	return true;// !getPathFinder().isLost(mSelf->getPosition());
}
