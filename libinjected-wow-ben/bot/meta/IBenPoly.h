#pragma once

#include <memory>
#include <string>

#include "pathfinder/IPathFinder.h"
#include "game/object/WowGuid128.h"
#include "game/Lua.h"

#include "bot/BaseWowBot.h"

class IBenPoly : public IWowBot {
public:
	WowBotContext(WowGame& game);
	virtual ~WowBotContext();

	bool shouldFlee() {
		// estimate threat of every aggroed
	}

protected:
	struct DangerZone {
		Vector3f position;
		float radius;
	};
	struct Hostile {
		WowGuid128 guid;
		DangerZone aggro;
	};
	struct HostileList {
		std::list<WowGuid128> units;
		std::list<WowGuid128> keepAway;
	};


	HostileList mAggroList;
	HostileList mInterestList;
	HostileList mHostileList;

	std::vector<DangerZone> mDangerZones;
};
