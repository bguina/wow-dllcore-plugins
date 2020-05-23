#pragma once

#include "WowPlayerObject.h"

class WowGame;

class WowActivePlayerObject : public WowPlayerObject
{
public:
	WowActivePlayerObject(const uint8_t* baseAddr);

	char canAttack(const WowGame& game, const WowUnitObject& target) const;

	bool isFriendly(const WowGame& game, const WowUnitObject& target) const;

	const uint32_t* getCamera(const WowGame& game);

	uint64_t interactWith(const WowGame& game, const uint32_t* targetGuid);
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowActivePlayerObject& obj
	)
{
	out << (WowPlayerObject)obj
		;
	return out;
}

