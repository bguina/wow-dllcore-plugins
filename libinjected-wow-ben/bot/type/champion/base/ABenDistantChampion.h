#pragma once

#include <string>
#include <list>

#include "ABenChampion.h"

class ABenDistantChampion : public ABenChampion {
public:
	ABenDistantChampion(const std::string& tag, ABenAgent* runagate);
	virtual ~ABenDistantChampion();

	void onUnitTap(const WowUnitObject& object) override;
	void onUnitAggro(const WowUnitObject& object) override;
	void onUnitKill(const WowUnitObject& object) override;

protected:
	bool onEvaluatedInFight() override;

	std::list<WowUnitObject> mKillList;
};

