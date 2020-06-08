#pragma once

#include <string>

#include "../base/ABenChampion.h"
#include "../base/ABenTamerChampion.h"

class BenHunterChampion : public ABenTamerChampion {
public:
	BenHunterChampion(ABenWowGameEvaluator* gameplay);
	BenHunterChampion(ABenWowGameEvaluator* gameplay, const std::string& tag);
	BenHunterChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* runagate);
	virtual ~BenHunterChampion();

	float getEngagementRange() const override;
	float getPreferredRange() const override;
	bool pull(const WowUnitObject& unit) override;
	bool engage(const WowUnitObject& unit) override;
	bool isPositionSatisfying(const WowVector3f& position) override;

	
protected:
	std::list<int> mSpells;
};