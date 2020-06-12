#pragma once

#include <string>

#include "ABenPatroller.h"
#include "pathfinder/LinearPathFinder.h"

class ABenWaypointsPatroller : public ABenPatroller {
public:
	ABenWaypointsPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenChampion* combatBot);
	ABenWaypointsPatroller(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot);
	virtual ~ABenWaypointsPatroller();

	bool handleWowMessage(ServerWowMessage& serverMessage) override;

protected:
	IPathFinder& getPathFinder() override;
	
	LinearPathFinder mPathFinder;
};

