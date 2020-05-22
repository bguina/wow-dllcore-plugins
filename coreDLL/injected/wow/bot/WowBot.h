#pragma once

#include <set>

#include "../../ISandboxPlugin.h"

#include "../WowVector3f.h"
#include "../../../debugger/FileDebugger.h"
#include "../objectmanager/WowObject.h"

class IPathFinder;
class WowGame;
class WowUnitObject;

class WowBot : public ISandboxPlugin
{
public:
	WowBot(WowGame& game);
	virtual ~WowBot();

	virtual void run();

	void pause(bool paused);
	bool isPaused() const;

	void loadLinearWaypoints(const std::vector<WowVector3f>& waypoints);

protected:
	WowGame& mGame;
	FileDebugger mDbg;
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
