#pragma once

#include <memory>
#include <set>
#include <string>

#include "../../base/ABenAgent.h"

class ABenChampion : public ABenAgent {
public:
	ABenChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate);
	virtual ~ABenChampion();
	
	bool runAway() override;

	bool onEvaluatedIdle() final override;

	virtual void onUnitTap(const WowUnitObject& object);
	virtual void onUnitAggro(const WowUnitObject& object);
	virtual void onUnitUnaggro(const WowUnitObject& object);
	virtual void onUnitKill(const WowUnitObject& object);

	virtual float getEngagementRange() const = 0;
	virtual float getPreferredRange() const = 0;
	virtual bool attack(const WowUnitObject& unit) = 0;

	virtual bool isPositionSatisfying(const WowVector3f& position) = 0;

protected:
	virtual bool onEvaluatedInFight() = 0;
	
	std::unique_ptr<ABenAgent> mRunagate;
	std::set<WowGuid128> mTargetingMe;
	std::set<WowGuid128> mTappedUnits;
};
