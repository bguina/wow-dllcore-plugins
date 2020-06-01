#pragma once

#include "ABenChampion.h"

class ABenRunagate : public ABenChampion {
public:
	ABenRunagate(WowGame& game, const std::string& tag, ABenRunagate* fallback);
	virtual ~ABenRunagate();

	// Game has objectmanager.getActivePlayer ready
	//virtual void _onGameplayResumed() = 0;
	//virtual void _onGameplayStopped() = 0;

	//virtual void _onTargetChanged() = 0;

	//virtual void _onCombatStart() = 0;
	//virtual void _onCombatEnd() = 0;

protected:

	std::list<WowUnitObject> mKillList;
};

