#pragma once

#include "../champion/base/ABenChampion.h"

class BenIdleAgent : public ABenChampion
{
public:
	BenIdleAgent();
	~BenIdleAgent();

	void onResume() override;
	void onPause() override;
	
	void onGamePlayStart() override;
	void onGamePlayStop() override;

	void onCombatStart() override;
	void onCombatEnd() override;

	bool isPositionSatisfying(const WowVector3f& position) override;
	bool pull(const WowUnitObject& unit) override;
	bool engage(const WowUnitObject& unit) override;

	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;
	float getEngagementRange() const override;
	float getPreferredRange() const override;

protected:
	bool onEvaluatedInFight() override;
};
