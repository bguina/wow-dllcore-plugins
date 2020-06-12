#pragma once

#include <memory>
#include <string>

#include "../../base/ABenAgent.h"

template<typename Evaluation> class IBenWowGameEvaluator;

class ABenChampion : public ABenAgent {
public:
	ABenChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag);
	virtual ~ABenChampion();
	
	bool runAway() override;
	virtual float getEngagementRange() const = 0;
	virtual float getPreferredRange() const = 0;

	virtual bool isInCombat() const;
	
	virtual bool pullCombat(const std::shared_ptr<WowUnitObject>& unit) = 0;
	
	virtual bool startCombat(const std::shared_ptr<WowUnitObject>& unit) = 0;

protected:
	bool onEvaluatedIdle() final override;
	
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

	virtual bool onEvaluatedAsChampion() = 0;

	std::list<std::shared_ptr<const WowUnitObject>> mCombatUnits;
	std::unique_ptr<ABenAgent> mRunagate;
};
