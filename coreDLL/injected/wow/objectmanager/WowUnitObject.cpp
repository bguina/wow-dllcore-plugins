#include "pch.h"

#include "../dump/WowGameOffsets.h"
#include "../WowGame.h"

#include "WowUnitObject.h"

WowUnitObject::WowUnitObject(
	const uint8_t* baseAddr
) : WowObject(baseAddr)
{}

WoWUnitClass WowUnitObject::getUnitClass() const {
	return *reinterpret_cast<const WoWUnitClass*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetClass);
}

std::string WowUnitObject::getUnitClassLabel() const {
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

int WowUnitObject::getUnitRace() const {
	return *(getDescriptor() + 0x158);
}

int WowUnitObject::getUnitLevel() const {
	return *(getDescriptor() + 0x134);
}

int WowUnitObject::getUnitHealth() const {
	return *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetHealth);
}

int WowUnitObject::getUnitMaxHealth() const {
	return *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetMaxHealth);
}

int WowUnitObject::getUnitEnergy() const {
	return *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetEnergy);
}

int WowUnitObject::getUnitMaxEnergy() const {
	return *reinterpret_cast<const uint32_t*>((getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetMaxEnergy));
}

uint64_t WowUnitObject::getTargetGuid() const {
	return *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetTargetGuid);
}
/*
void WowUnitObject::moveTo(WowGame& game, const WowVector3f& destination) {
	int angle = getPosition().getFacingDegreesTo(destination);
	int delta = getPosition().getFacingDeltaDegrees(getFacingDegrees(), destination);
	int anglePrecision = 10;

	auto windowController = game.getWindowController();

	windowController->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
	windowController->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
	// move forward if approximately on the right facing
	windowController->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);
}
*/