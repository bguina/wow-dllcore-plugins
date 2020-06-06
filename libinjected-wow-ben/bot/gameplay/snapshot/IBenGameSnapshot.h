#pragma once

#include <list>
#include <memory>

#include "game/WowObjects.h"
#include "object/WowUnitSnapshot.h"

class IBenGameSnapshot  {
public:
	using Timestamp = unsigned long long;
	using UnitList = std::list<std::shared_ptr<WowUnitSnapshot>>;

	virtual ~IBenGameSnapshot() = default;

	virtual Timestamp getTimestamp() const = 0;
	virtual long getNetworkLatencyMs() const = 0;

	virtual const UnitList& listHostiles() const = 0;
	virtual const UnitList& listNonHostiles() const = 0;

	virtual const WowPlayerObject* getSelf() const = 0;

	bool operator<(const IBenGameSnapshot& b) const
	{
		return getTimestamp() < b.getTimestamp();
	}
};
