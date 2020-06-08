#include "ABenPatroller.h"

ABenPatroller::ABenPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* combatBot) :
	ABenPatroller(gameplay, tag, nullptr, combatBot)
{
}

ABenPatroller::ABenPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot) :
	ABenAgent(gameplay, tag),
	mTraveler(travelBot),
	mChamp(combatBot)
{
	if (nullptr != travelBot) mNestedAgents.push_back(travelBot);
	if (nullptr != combatBot) mNestedAgents.push_back(combatBot);
}

ABenPatroller::~ABenPatroller() = default;



bool ABenPatroller::onEvaluatedIdle()
{
	FileLogger dbg(mDbg, "onEvaluatedIdle");

	if (nullptr == mChamp)
	{
		dbg << dbg.e() << "no champ !" << dbg.endl();

		return false;
	}

	if (mChamp->onEvaluate())
		return true;


	
	{
		Vector3f nextWaypoint;
		// no combat to handle

		std::list<std::shared_ptr<WowUnitObject>> hostiles(mGame->getObjectManager().allOfType<WowUnitObject>(WowObjectType::Unit));
		hostiles.sort([&](const auto& a, const auto& b)->bool {
			return evaluatePatrolRelativeThreat(*a) < evaluatePatrolRelativeThreat(*b);
			//return posA.getDistanceTo(nextWaypoint) * mSelf->getPosition().getDistanceTo(posA) <
				//posB.getDistanceTo(nextWaypoint) * mSelf->getPosition().getDistanceTo(posB);
			});


		int i = 0;
		for (auto it = hostiles.begin(); it != hostiles.end(); ++it)
		{
			++i;
			dbg << dbg.i() << "threat #" << i << ": " << (*it)->getPosition().getDistanceTo(mSelf->getPosition()) << " level " << (*it)->getLevel() << dbg.endl();
		}

		
		if (getPathFinder().getNextPosition(mSelf->getPosition(), nextWaypoint))
		{
			//mGameplay->estimatePathThreat(mGame, position, time)
			// list all mobs around the path, listed by 1/(distance to us * nearest distance to path segment)
			//std::vector<std::shared_ptr<WowUnitSnapshot>> hostiles(mGameplay->front()->getHostileList().begin(), mGameplay->front()->getHostileList().end());


			//if (patrolShouldAttack(*unit))
			//{
			//	if (mChamp->wouldKill(unit))
			//	{
			//		
			//	}
			//	else
			//	{
			//		mDangerList.push_back(unit);
			//	}
			//}
			//else
			//{
			//	
			//}

			//mSelf->moveTo(*mGame, nextWaypoint);
			return true;
		}
		dbg << dbg.e() << "could not find any path to patrol" << dbg.endl();
	}

	return true;// !getPathFinder().isLost(mSelf->getPosition());
}
