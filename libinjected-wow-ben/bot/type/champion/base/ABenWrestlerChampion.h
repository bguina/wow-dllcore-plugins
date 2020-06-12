#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

class IBenGameEvaluator;

class ABenWrestlerChampion : public ABenChampion {
public:
	ABenWrestlerChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag);
	virtual ~ABenWrestlerChampion();

protected:
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

	bool onEvaluatedAsChampion() final override;
	virtual bool onEvaluatedAsWrestlerChampion() = 0;

	std::list<WowUnitObject> mKillList;
};

