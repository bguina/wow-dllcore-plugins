#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "Hexdump.h"
#include "Hexsearch.h"
#include "MemoryObject.h"

class WowObject : public MemoryObject
{
public:
	WowObject(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr)
	{}

	enum Type {
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

	uint64_t getGuid() {
		return *(uint64_t*)(baseAddress() + 0x04);
	}

	uint64_t* getGuidPtr() {
		return (uint64_t*)(baseAddress() + 0x04);
	}

	//		StorageField = 0x10,//good-33526
	//		ObjectType = 0x20,//good-33526
	//		NextObject = 0x70,//good-33526
	//		FirstObject = 0x18,//good-33526
	//		LocalGUID = 0x58, //good-33526

	Type getType() const {
		return static_cast<WowObject::Type>(*(baseAddress() + 0x20));
	}

	std::string getTypeLabel() const {
		switch (getType()) {
		case Object:return  "Object";
		case Item:return "Item";
		case Container:return  "Container";
		case AzeriteEmpoweredItem: "AzeriteEmpoweredItem";
		case AzeriteItem: "AzeriteItem";
		case Unit:return  "Unit";
		case Player:return  "Player";
		case ActivePlayer:return  "ActivePlayer";
		case GameObject:return  "GameObject";
		case DynamicObject: return "DynamicObject";
		case Corpse:return  "Corpse";
		case AreaTrigger: return "AreaTrigger";
		case Scene:return  "Scene";
		case Conversation: return  "Conversation";
		case AiGroup: return  "AiGroup";
		case Scenario: return  "Scenario";
		case Loot: return  "Loot";
		case Invalid: return  "Invalid";
		default: return "Unknown";
		}
	}

	float getX() const {
		return *(float*)(baseAddress() + 0x1600);
	}

	float getY() const {
		return *(float*)(baseAddress() + 0x1604);
	}

	float getZ() const {
		return *(float*)(baseAddress() + 0x1608);
	}

	void* vtableAt(unsigned index) {
		return ((void**)baseAddress())[index];
	}

	void move() {
		//void(__thiscall* CGPlayer_C__ClickToMove)(void * , uint8_t ,uint64_t *, float *, float ) = vtableAt(0);
	}
};


//typedef bool __thiscall (*CGPlayer_C__ClickToMove)(int *, int, int *, int *, float);

inline std::ostream& operator<<(
	std::ostream& out,
	const WowObject& obj
	)
{
	out << obj.getTypeLabel() << "@" << obj.getX() << "," << obj.getY() << "," << obj.getZ() << std::endl;
	if (false && WowObject::Type::ActivePlayer == obj.getType()) {
		out << Hexdump(*reinterpret_cast<const void* const*>(obj.baseAddress()), 16 * 5) << std::endl;
		//out << Hexsearch<uint64_t>(*reinterpret_cast<const void* const*>(obj.baseAddress()), 0x8CAE30, 16 * 5) << std::endl;
	}
	return out;
}