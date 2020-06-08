#pragma once

#include <string>

#include "ABenPatroller.h"
#include "pathfinder/LinearPathFinder.h"

class ABenWaypointsPatroller : public ABenPatroller {
public:
	ABenWaypointsPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* combatBot);
	ABenWaypointsPatroller(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~ABenWaypointsPatroller();

	bool handleWowMessage(ServerWowMessage& serverMessage) override;

protected:
	IPathFinder& getPathFinder() override;
	
	LinearPathFinder mPathFinder;
};

