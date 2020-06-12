#pragma once

#include <vector>
#include "game/world/object/WowUnitObject.h"

class WowBaseEvaluation
{
public:
	using UnitList = std::vector<WowGuid128>;
	bool gameEntered;
	bool gameLeft;
	bool combatEntered;
	bool combatLeft;
	UnitList popList;
	UnitList depopList;
	UnitList deathList;
	UnitList aggroList;
	UnitList aggroLostList;
	UnitList tappedList;
	UnitList untappedList;
	
	void reset()
	{
		gameEntered = false;
		gameLeft = false;
		combatEntered = false;
		combatLeft = false;
		popList.clear();
		depopList.clear();
		deathList.clear();
		aggroList.clear();
		aggroLostList.clear();
	}
};
