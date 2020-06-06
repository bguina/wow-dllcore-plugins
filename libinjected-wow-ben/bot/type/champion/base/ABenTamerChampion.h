#pragma once

#include <string>
#include <list>

#include "ABenDistantChampion.h"
#include "ABenChampion.h"

class ABenTamerChampion : public ABenDistantChampion {
public:
	ABenTamerChampion(const std::string& tag, ABenAgent* runagate);
	virtual ~ABenTamerChampion();
	
	void onUnitTap(const WowUnitObject& object) override;
	void onUnitAggro(const WowUnitObject& object) override;
	void onUnitKill(const WowUnitObject& object) override;

protected:
	bool onEvaluatedInFight() override;
	
	std::list<WowUnitObject> mKillList;
};
