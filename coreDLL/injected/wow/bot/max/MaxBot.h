#pragma once

#include <set>

#include "../WowBot.h"
#include "../../../../pathfinder/IPathWaypointsConsumer.h"
#include "../../objectmanager/WowGuid64.h"

class WowUnitObject;

class WowMaxBot : public WowBot, public IPathWaypointsConsumer
{
public:
	WowMaxBot(WowGame& game);
	virtual ~WowMaxBot();

	virtual void logDebug() const override;

	virtual void run() override;

	virtual void loadPathWaypoints(const std::vector<Vector3f>& waypoints) override;

protected:
	std::unique_ptr<IPathFinder> mPathFinder;
	std::shared_ptr<const WowUnitObject> mTargetUnit;
	std::set<WowGuid64> mBlacklistedGuids;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowMaxBot& obj
	)
{
	out << dynamic_cast<const WowBot&>(obj);
	return out;
}
