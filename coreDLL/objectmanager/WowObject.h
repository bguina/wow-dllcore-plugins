#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "../memutils/Hexdump.h"
#include "../memutils/Hexsearch.h"
#include "../MemoryObject.h"
#include "../Vector3f.h"

class WowObject : public MemoryObject
{
public:
	WowObject(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr)
	{}

	enum Type : uint8_t {
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

	const uint8_t* getDescriptor() const {
		return *(uint8_t**)(getBaseAddress() + 0x10);
	}

	uint64_t getGuid() const {
		return *(uint64_t*)(getBaseAddress() + 0x58);
	}

	const uint32_t*getGuidPointer() const {
		return (uint32_t*)(getBaseAddress() + 0x58);
	}

	//		StorageField = 0x10,//good-33526
	//		ObjectType = 0x20,//good-33526
	//		NextObject = 0x70,//good-33526
	//		FirstObject = 0x18,//good-33526
	//		LocalGUID = 0x58, //good-33526

	Type getType() const {
		return (WowObject::Type)(*(getBaseAddress() + 0x20));
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

	const Vector3f& getPosition() const {
		return *((Vector3f*)(getBaseAddress() + 0x1600));
	}

	float getX() const {
		return ((Vector3f*)(getBaseAddress() + 0x1600))->x;
	}

	float getY() const {
		return ((Vector3f*)(getBaseAddress() + 0x1600))->y;
	}

	float getZ() const {
		return ((Vector3f*)(getBaseAddress() + 0x1600))->z;
	}

	float getFacingRadians() const {
		return *(float*)(getBaseAddress() + 0x1610);
	}

	int getFacingDegrees() const {
		const double PI = 3.141592653589793;
		return (int)(getFacingRadians() * 180 / PI);
	}

private:
	void* vtableAt(unsigned index) {
		return ((void**)getBaseAddress())[index];
	}
};

//typedef bool __thiscall (*CGPlayer_C__ClickToMove)(int *, int, int *, int *, float);

inline std::ostream& operator<<(
	std::ostream& out,
	const WowObject& obj
	)
{
	out << "[WowObject@" << (void*)obj.getBaseAddress() << "]" << std::endl;
	out << obj.getTypeLabel() << "[GUID 0x" << (void*)obj.getGuid() << "]@" << obj.getX() << "," << obj.getY() << " facing " << obj.getFacingRadians();

	//if (false && WowObject::Type::ActivePlayer == obj.getType()) {
	//	out << Hexdump(*reinterpret_cast<const void* const*>(obj.getBaseAddress()), 16 * 5) << std::endl;
	//	//out << Hexsearch<uint64_t>(*reinterpret_cast<const void* const*>(obj.getBaseAddress()), 0x8CAE30, 16 * 5) << std::endl;
	//}

	return out;
}

