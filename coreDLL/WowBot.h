#pragma once

#include <set>

#include "debugger/FileDebugger.h"

class IPathFinder;
class WowGame;

class WowBot
{
public:
	WowBot(WowGame& game);

	~WowBot();

	void run();

	const WowGame& getGame() const;

	void setBotStarted(bool started);

	void loadLinearWaypoints(const std::vector<Vector3f>& waypoints);

protected:
	WowGame& mGame;
	FileDebugger mDbg;
	bool mBotStarted;
	std::unique_ptr<IPathFinder> mPathFinder;
	std::shared_ptr<const WowUnitObject> mCurrentUnitTarget;
	std::set<WowGuid64> mBlacklistedGuids;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowBot& obj
	)
{
	out << "[WowBot]";
	// TODO print navigator state
	return out;
}
