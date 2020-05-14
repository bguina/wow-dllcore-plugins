#pragma once

#include "WowObject.h"

class WowUnitObject : public WowObject
{
public:
	WowUnitObject(
		const uint8_t* baseAddr
	) : WowObject(baseAddr)
	{}

	enum WoWUnitClass {
		None = 0,
		Warrior = 1,
		Paladin = 2,
		Hunter = 3,
		Rogue = 4,
		Priest = 5,
		DeathKnight = 6,
		Shaman = 7,
		Mage = 8,
		Warlock = 9,
		Druid = 11
	};

	const uint8_t* getUnitDescriptor() const {
		return baseAddress() + 0x10;
	}

	const WoWUnitClass getUnitClass() const {
		return (WoWUnitClass)*(getUnitDescriptor() + 0xD1);
	}

	const int getUnitRace() const {
		return *(getUnitDescriptor() + 0x158);
	}

	const uint64_t getTargetGuid() const {
		return *(getUnitDescriptor() + 0x9C);
	}
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowUnitObject& obj
	)
{
	out << (WowObject)obj;
	out << "Object is unit with class=" << obj.getUnitClass() << " race=" << obj.getUnitRace() << " target=" << obj.getTargetGuid() << std::endl;
	return out;
}

