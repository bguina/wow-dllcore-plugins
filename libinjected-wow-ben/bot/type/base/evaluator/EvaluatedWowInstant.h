#pragma once

#include <vector>
#include "game/object/WowUnitObject.h"

class EvaluatedWowInstant
{
public:
	using UnitList = std::vector<WowGuid128>;
	bool gameEntered;
	bool gameLeft;
	bool combatEntered;
	bool combatLeft;
	UnitList appearList;
	UnitList vanishList;
	UnitList deathList;
	UnitList aggroList;
	UnitList aggroLostList;

	void reset()
	{
		gameEntered = false;
		gameLeft = false;
		combatEntered = false;
		combatLeft = false;
		appearList.clear();
		vanishList.clear();
		deathList.clear();
		aggroList.clear();
		aggroLostList.clear();
	}
};
