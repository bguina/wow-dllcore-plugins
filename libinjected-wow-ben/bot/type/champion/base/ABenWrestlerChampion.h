#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

class ABenWrestlerChampion : public ABenChampion {
public:
	ABenWrestlerChampion(const std::string& tag, ABenAgent* runagate);
	virtual ~ABenWrestlerChampion();

	void onUnitTap(const WowUnitObject& object) override;
	void onUnitAggro(const WowUnitObject& object) override;
	void onUnitKill(const WowUnitObject& object) override;

protected:
	bool onEvaluatedInFight() override;

	std::list<WowUnitObject> mKillList;
};

