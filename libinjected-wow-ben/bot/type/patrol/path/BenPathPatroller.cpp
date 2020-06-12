#include "BenPathPatroller.h"

BenPathPatroller::BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, ABenChampion* combatBot) :
	BenPathPatroller(gameplay, "BenPathPatroller", nullptr, combatBot)
{
}

BenPathPatroller::BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenChampion* combatBot) :
	BenPathPatroller(gameplay, tag, nullptr, combatBot)
{
}

BenPathPatroller::BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot) :
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

bool BenPathPatroller::patrolShouldAttack(const WowUnitObject & unit) const
{
	return true;
}

float BenPathPatroller::evaluatePatrolRelativeThreat(const Vector3f & nextWaypoint, const WowUnitObject & unit)
{
	const auto selfDistanceToUnit(mSelf->getDistanceTo(unit));

	if (selfDistanceToUnit <= mChamp->getEngagementRange() * 1.4)
	{
		if (mSelf->canAttack(unit))
		{
			const auto posA(unit.getPosition());

			return (unit.getLevel() + 5 - mSelf->getLevel()) * 100 / selfDistanceToUnit;
		}
	}
	return 0;
}
