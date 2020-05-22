#pragma once

#include <set>

#include "../../bot/AGameBot.h"

#include "../WowVector3f.h"
#include "../objectmanager/WowObject.h"

class IPathFinder;
class WowGame;
class WowUnitObject;

class WowBot : public AGameBot<WowGame>
{
public:
	WowBot(WowGame& game);
	~WowBot();

	virtual void run();

	void pause(bool paused);
	bool isPaused() const;

	void loadLinearWaypoints(const std::vector<WowVector3f>& waypoints);

protected:
	bool mPaused;
	std::unique_ptr<IPathFinder> mPathFinder;
	std::shared_ptr<const WowUnitObject> mCurrentUnitTarget;
	std::set<WowGuid64> mBlacklistedGuids;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowBot& obj
	)
{
	out << "[WowBot:" << (obj.isPaused() ? "paused" : "running") << "]";
	return out;
}
