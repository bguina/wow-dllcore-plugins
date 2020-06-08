#pragma once

#include "../../travel/ABenTraveler.h"
#include "../../champion/base/ABenChampion.h"
#include "pathfinder/IPathFinder.h"

class ABenPatroller : public ABenAgent {
public:
	ABenPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* combatBot);
	ABenPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~ABenPatroller();

	virtual void onPathFound() = 0;
	virtual void onPathLost() = 0;
	
protected:
	bool onEvaluatedIdle() override;
	
	virtual float evaluatePatrolRelativeThreat(const WowUnitObject& unit) = 0;
	virtual bool patrolShouldAttack(const WowUnitObject& unit) const = 0;

	std::unique_ptr<ABenTraveler> mTraveler;
	std::unique_ptr<ABenChampion> mChamp;
	
	virtual IPathFinder& getPathFinder() = 0;

	IBenGameSnapshot::UnitList mBlackList;
	IBenGameSnapshot::UnitList mDangerList;
};
