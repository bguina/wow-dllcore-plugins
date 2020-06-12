#pragma once

#include "../../travel/ABenTraveler.h"
#include "../../champion/base/ABenChampion.h"
#include "pathfinder/IPathFinder.h"

class ABenPatroller : public ABenAgent {
public:
	ABenPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenChampion* combatBot);
	ABenPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~ABenPatroller();

	void setCombatBot(ABenChampion* agent);

protected:
	virtual void onPathFound() = 0;
	virtual void onPathLost() = 0;
	
	bool onEvaluatedIdle() override;

	/* Threat indice between -1 (haven) and 1 (hardcore danger)*/
	virtual float evaluatePatrolRelativeThreat(const Vector3f& nextWaypoint, const WowUnitObject& unit) = 0;
	virtual bool patrolShouldAttack(const WowUnitObject& unit) const = 0;

	std::unique_ptr<ABenTraveler> mTraveler;
	std::unique_ptr<ABenChampion> mChamp;
	
	virtual IPathFinder& getPathFinder() = 0;

	IBenWowGameSnapshot::UnitList mBlackList;
	IBenWowGameSnapshot::UnitList mDangerList;
};
