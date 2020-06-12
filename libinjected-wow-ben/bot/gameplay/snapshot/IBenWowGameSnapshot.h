#pragma once

#include <list>
#include <memory>

#include "game/world/WowObjects.h"
#include "object/WowUnitSnapshot.h"

class IBenWowGameSnapshot {
public:
	using Timestamp = unsigned long long;
	using UnitList = std::list<std::shared_ptr<WowUnitSnapshot>>;

	virtual ~IBenWowGameSnapshot() = default;

	virtual Timestamp getTimestamp() const = 0;
	virtual long getNetworkLatencyMs() const = 0;

	virtual bool isInGame() const = 0;
	virtual bool isInCombat() const = 0;
	
	enum class Faction
	{
		FactionHostile,
		FactionNeutral,
		FactionFriendly
	};
	virtual const UnitList& getUnitList(Faction faction) const = 0;
	
	virtual std::shared_ptr<const WowUnitSnapshot> getUnitByGuid(WowGuid128 guid) const = 0;
	
	virtual const WowPlayerObject* getSelf() const = 0;

	bool operator<(const IBenWowGameSnapshot& b) const
	{
		return getTimestamp() < b.getTimestamp();
	}
};
