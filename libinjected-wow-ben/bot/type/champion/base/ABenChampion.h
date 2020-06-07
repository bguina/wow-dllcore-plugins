#pragma once

#include <memory>
#include <string>

#include "../../base/ABenAgent.h"

class ABenChampion : public ABenAgent {
public:
	ABenChampion(IBenGameplay* gameplay, const std::string& tag);
	ABenChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate);
	virtual ~ABenChampion();
	
	bool runAway() override;

	virtual void onUnitTap(const std::shared_ptr<const WowUnitObject>& object);
	virtual void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object);

	virtual float getEngagementRange() const = 0;
	virtual float getPreferredRange() const = 0;
	
	virtual bool pull(const WowUnitObject& unit) = 0;
	virtual bool engage(const WowUnitObject& unit) = 0;

	virtual bool isPositionSatisfying(const WowVector3f& position) = 0;

protected:
	bool updateFromSnapshot(const std::shared_ptr<const IBenGameSnapshot>& snapshot) override;
	bool onEvaluatedIdle() final override;

	virtual bool onEvaluatedInFight() = 0;

	std::unique_ptr<ABenAgent> mRunagate;
};
