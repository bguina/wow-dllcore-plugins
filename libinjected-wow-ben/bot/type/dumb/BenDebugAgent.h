#pragma once

#include "../champion/base/ABenChampion.h"

class BenDebugAgent : public ABenChampion
{
public:
	BenDebugAgent();
	BenDebugAgent(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay);
	virtual ~BenDebugAgent();

	void onResume() override;
	void onPause() override;
	
	void onGamePlayStart() override;
	void onGamePlayStop() override;

	void onCombatStart() override;
	void onCombatEnd() override;

	bool pullCombat(const std::shared_ptr<WowUnitObject>& unit) override;
	bool startCombat(const std::shared_ptr<WowUnitObject>& unit) override;
	
	float getEngagementRange() const override;
	float getPreferredRange() const override;

protected:
	bool onEvaluatedAsChampion() override;
	
	void onUnitPop(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDepop(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitUntap(const std::shared_ptr<const WowUnitObject>& object) override;
};
