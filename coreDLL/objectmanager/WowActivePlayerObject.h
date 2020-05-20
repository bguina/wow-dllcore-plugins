#pragma once

#include "WowPlayerObject.h"

class WowGame;

class WowActivePlayerObject : public WowPlayerObject
{
public:
	WowActivePlayerObject(
		const uint8_t* baseAddr
	) : WowPlayerObject(baseAddr)
	{}

	char canAttack(const WowGame& game, const WowUnitObject& target) const;

	bool isFriendly(const WowGame& game, const WowUnitObject& target) const;

};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowActivePlayerObject& obj
	)
{
	out << (WowPlayerObject)obj;
	return out;
}

