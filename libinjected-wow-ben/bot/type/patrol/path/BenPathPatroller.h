#pragma once

#include <string>

#include "BenPathPatroller.h"
#include "../base/ABenWaypointsPatroller.h"

class BenPathPatroller : public ABenWaypointsPatroller {
public:
	BenPathPatroller(IBenGameplay* gameplay, ABenChampion* combatBot);
	BenPathPatroller(IBenGameplay* gameplay, const std::string& tag, ABenChampion* combatBot);
	BenPathPatroller(IBenGameplay* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~BenPathPatroller();

	void onPathFound() override;
	void onPathLost() override;
	
protected:
	bool patrolShouldAttack(const WowUnitObject& unit) const override;
	float evaluatePatrolRelativeThreat(const WowUnitObject& unit) override;

};
