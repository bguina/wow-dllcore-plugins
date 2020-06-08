#include "BenPathPatroller.h"

BenPathPatroller::BenPathPatroller(ABenWowGameEvaluator* gameplay, ABenChampion* combatBot) :
	BenPathPatroller(gameplay, "BenPathPatroller", nullptr, combatBot)
{
}

BenPathPatroller::BenPathPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* combatBot) :
	BenPathPatroller(gameplay, tag, nullptr, combatBot)
{
}

BenPathPatroller::BenPathPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot) :
	ABenWaypointsPatroller(gameplay, tag, travelBot, combatBot)
{
}

BenPathPatroller::~BenPathPatroller() = default;

void BenPathPatroller::onPathFound()
{
	
}

void BenPathPatroller::onPathLost()
{
	
}

bool BenPathPatroller::patrolShouldAttack(const WowUnitObject& unit) const
{
	return true;
}

float BenPathPatroller::evaluatePatrolRelativeThreat(const WowUnitObject& unit)
{
	//posA.getDistanceTo(nextWaypoint)* mSelf->getPosition().getDistanceTo(posA) <
		//posB.getDistanceTo(nextWaypoint) * mSelf->getPosition().getDistanceTo(posB);
	return 0;
}
