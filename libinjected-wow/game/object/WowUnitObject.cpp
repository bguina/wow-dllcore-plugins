#include "../dump/WowGameOffsets.h"
#include "../WowGame.h"

#include "WowUnitObject.h"

WowUnitObject::WowUnitObject(
	const uint8_t* baseAddr
) : WowObject(baseAddr)
{}

WowUnitClass WowUnitObject::getUnitClass() const {
	return *reinterpret_cast<const WowUnitClass*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetClass);
}

std::string WowUnitObject::getUnitClassLabel() const {
	switch (getUnitClass()) {
	case WowUnitClass::Warrior: return "Warrior";
	case WowUnitClass::Paladin: return "Paladin";
	case WowUnitClass::Hunter: return "Hunter";
	case WowUnitClass::Rogue: return "Rogue";
	case WowUnitClass::Priest: return "Priest";
	case WowUnitClass::DeathKnight: return "DeathKnight";
	case WowUnitClass::Shaman: return "Shaman";
	case WowUnitClass::Mage: return "Mage";
	case WowUnitClass::Warlock: return "Warlock";
	case WowUnitClass::Druid: return "Druid";
	default: return "Unit";
	}
}

WowUnitRace WowUnitObject::getUnitRace() const {
	return *reinterpret_cast<const WowUnitRace*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetRace);
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

float WowUnitObject::getUnitHealthPercentage() const {
	uint32_t currentHealth = *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetHealth);
	uint32_t maxHealth = *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetMaxHealth);
	return (currentHealth * 100.00f / maxHealth);
}

int WowUnitObject::getUnitEnergy() const {
	return *reinterpret_cast<const uint32_t*>(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetEnergy);
}

int WowUnitObject::getUnitMaxEnergy() const {
	return *reinterpret_cast<const uint32_t*>((getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetMaxEnergy));
}

bool WowUnitObject::isInCombat() const {
	return *reinterpret_cast<const uint32_t*>((getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetUnitDynamicflags)) & (uint32_t)WowUnitDynamicFlags::isInCombat;
}

bool WowUnitObject::isLootable() const {
	return *reinterpret_cast<const uint32_t*>((getDescriptor() + WowGameOffsets::WowObject::DescriptorOffsetObjectDynamicflags)) & (uint32_t)WowObjectDynamicFlags::Lootable;
}

WowGuid128  WowUnitObject::getSummonedBy() const {
	return ((WowGuid128*)(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetSummonedBy))[0];
}

WowGuid128 WowUnitObject::getTargetGuid() const {
	return ((WowGuid128*)(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetTargetGuid))[0];
}

WowGuid128* WowUnitObject::getTargetGuidPtr() const {
	return ((WowGuid128*)(getDescriptor() + WowGameOffsets::WowUnitObject::DescriptorOffsetTargetGuid));
}

void WowUnitObject::moveTo(WowGame& game, const WowVector3f& destination) {
	int delta = getPosition().getFacingDeltaDegrees(getFacingDegrees(), destination);
	int anglePrecision = 10;

	auto windowController = game.getWindowController();

	windowController->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
	windowController->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
	// move forward if approximately on the right facing
	windowController->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);
}
