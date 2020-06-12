#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

template<typename Evaluation> class IBenWowGameEvaluator;
class IBenGameEvaluator;

class ABenDistantChampion : public ABenChampion {
public:
	ABenDistantChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag);
	virtual ~ABenDistantChampion();

protected:
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

	bool onEvaluatedAsChampion() final override;
	virtual bool onEvaluatedAsDistantChampion() = 0;

	std::list<WowUnitObject> mKillList;
};

