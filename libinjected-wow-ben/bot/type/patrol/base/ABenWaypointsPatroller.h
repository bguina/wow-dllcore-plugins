#pragma once

#include <string>

#include "ABenPatroller.h"
#include "pathfinder/LinearPathFinder.h"

class ABenWaypointsPatroller : public ABenPatroller {
public:
	ABenWaypointsPatroller(IBenGameplay* gameplay, const std::string& tag, ABenChampion* combatBot);
	ABenWaypointsPatroller(IBenGameplay* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~ABenWaypointsPatroller();

	bool handleWowMessage(ServerWowMessage& serverMessage) override;

protected:
	IPathFinder& getPathFinder() override;
	
	LinearPathFinder mPathFinder;
};

