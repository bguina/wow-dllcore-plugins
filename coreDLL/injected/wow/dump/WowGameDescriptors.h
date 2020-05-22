#pragma once

#include <cstdint>

enum class WowObjectType : uint8_t {
	Object = 0,
	Item = 1,
	Container = 2,
	AzeriteEmpoweredItem = 3,
	AzeriteItem = 4,
	Unit = 5,
	Player = 6,
	ActivePlayer = 7,
	GameObject = 8,
	DynamicObject = 9,
	Corpse = 10,
	AreaTrigger = 11,
	Scene = 12,
	Conversation = 13,
	AiGroup = 14,
	Scenario = 15,
	Loot = 16,
	Invalid = 17
};

enum class WowUnitClass : uint8_t {
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

enum class WowUnitRace : uint8_t {
	None = 0,

};
