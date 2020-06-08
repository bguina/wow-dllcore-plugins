#pragma once

#include <string>
#include <list>

#include "ABenDistantChampion.h"
#include "ABenChampion.h"

class ABenWowGameEvaluator;

class ABenTamerChampion : public ABenDistantChampion {
public:
	ABenTamerChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenAgent* runagate);
	virtual ~ABenTamerChampion();
	
	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

protected:
	bool onEvaluatedInFight() override;
	
	std::list<WowUnitObject> mKillList;
};
