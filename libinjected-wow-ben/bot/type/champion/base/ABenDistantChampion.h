#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

class ABenDistantChampion : public ABenChampion {
public:
	ABenDistantChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate);
	virtual ~ABenDistantChampion();

	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

protected:
	bool onEvaluatedInFight() override;

	std::list<WowUnitObject> mKillList;
};

