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

	WoWUnitClass getUnitClass() const {
		return static_cast<WoWUnitClass>(*(getDescriptor() + 0xD1));
	}

	std::string getUnitClassLabel() const {
		switch (getUnitClass()) {
		case Warrior: return "Warrior";
		case Paladin: return "Paladin";
		case Hunter: return "Hunter";
		case Rogue: return "Rogue";
		case Priest: return "Priest";
		case DeathKnight: return "DeathKnight";
		case Shaman: return "Shaman";
		case Mage: return "Mage";
		case Warlock: return "Warlock";
		case Druid: return "Druid";
		default: return "Unit";
		}
	}

	int getUnitRace() const {
		return *(getDescriptor() + 0x158);
	}

	int getUnitLevel() const {
		return *(getDescriptor() + 0x134);
	}

	int getUnitHealth() const {
		return *reinterpret_cast<const uint32_t*>(getDescriptor() + 0xDC);
	}

	int getUnitMaxHealth() const {
		return *reinterpret_cast<const uint32_t*>(getDescriptor() + 0xFC);
	}

	int getUnitEnergy() const {
		return *reinterpret_cast<const uint32_t*>(getDescriptor() + 0xE4);
	}

	int getUnitMaxEnergy() const {
		return *reinterpret_cast<const uint32_t*>((getDescriptor() + 0x104));
	}

	uint64_t getTargetGuid() const {
		return *reinterpret_cast<const uint32_t*>(getDescriptor() + 0x00);
	}

	//bool canSee(const WowGame& game, const Vector3f& position) {
	//	return game.traceLine(position, getPosition(), 0x100151);
//	}

};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowUnitObject& obj
	)
{
	out << (WowObject)obj
		<< ": [LVL" << obj.getUnitLevel() << "]" << obj.getUnitClassLabel()
		<< " health=" << obj.getUnitHealth() << "/" << obj.getUnitMaxHealth()
		<< " energy=" << obj.getUnitEnergy() << "/" << obj.getUnitMaxEnergy();
	return out;
}

