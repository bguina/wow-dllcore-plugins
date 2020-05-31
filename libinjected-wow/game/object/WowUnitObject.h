#pragma once

#include "../dump/WowGameDescriptors.h"
#include "WowObject.h"

class WowGame;

class WowUnitObject : public WowObject
{
public:
	WowUnitObject(const uint8_t* baseAddr);

	WowUnitClass getUnitClass() const;

	std::string getUnitClassLabel() const;

	WowUnitRace getUnitRace() const;

	int getUnitLevel() const;

	int getUnitHealth() const;

	int getUnitMaxHealth() const;

	float getUnitHealthPercentage() const;

	int getUnitEnergy() const;

	int getUnitMaxEnergy() const;

	bool isInCombat() const;

	bool isLootable() const;

	WowGuid128 getSummonedBy() const;

	WowGuid128 getTargetGuid() const;

	WowGuid128* getTargetGuidPtr() const;

	void moveTo(WowGame& game, const WowVector3f& destination);
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowUnitObject& obj
	)
{
	out << (WowObject)obj
		<< ": [LVL" << obj.getUnitLevel() << "]" << obj.getUnitClassLabel()
		<< ": [SummonedBy Guid = " << obj.getSummonedBy().upper() << obj.getSummonedBy().lower()
		<< " health=" << obj.getUnitHealth() << "/" << obj.getUnitMaxHealth()
		<< " energy=" << obj.getUnitEnergy() << "/" << obj.getUnitMaxEnergy();
	return out;
}
