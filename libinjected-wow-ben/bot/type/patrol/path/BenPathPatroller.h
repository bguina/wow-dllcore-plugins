#pragma once

#include <string>

#include "BenPathPatroller.h"
#include "../base/ABenWaypointsPatroller.h"

class BenPathPatroller : public ABenWaypointsPatroller {
public:
	BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, ABenChampion* combatBot);
	BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenChampion* combatBot);
	BenPathPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~BenPathPatroller();

	void onPathFound() override;
	void onPathLost() override;
	
protected:
	bool patrolShouldAttack(const WowUnitObject& unit) const override;
	float evaluatePatrolRelativeThreat(const Vector3f& nextWaypoint, const WowUnitObject& unit) override;

};
