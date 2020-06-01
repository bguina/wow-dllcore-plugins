#pragma once

#include "../travel/ABenTraveller.h"
#include "../champion/ABenChampion.h"

class ABenPatroller : public ABen {
public:
	ABenPatroller(const std::string& tag, ABenTraveller* travelBot, ABenChampion* combatBot);
	virtual ~ABenPatroller();


	// Game has objectmanager.getActivePlayer ready
	//virtual void _onGameplayResumed() = 0;
	//virtual void _onGameplayStopped() = 0;

	//virtual void _onTargetChanged() = 0;

	//virtual void _onCombatStart() = 0;
	//virtual void _onCombatEnd() = 0;

protected:
	std::unique_ptr<ABenTraveller> mTravel;
	std::unique_ptr<ABenChampion> mChamp;

	std::list<WowUnitObject> mKillList;
};

