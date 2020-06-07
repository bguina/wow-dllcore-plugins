#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

class ABenWrestlerChampion : public ABenChampion {
public:
	ABenWrestlerChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate);
	virtual ~ABenWrestlerChampion();

	void onUnitTap(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitAggro(const std::shared_ptr<const WowUnitObject>& object) override;
	void onUnitDeath(const std::shared_ptr<const WowUnitObject>& object) override;

protected:
	bool onEvaluatedInFight() override;

	std::list<WowUnitObject> mKillList;
};

