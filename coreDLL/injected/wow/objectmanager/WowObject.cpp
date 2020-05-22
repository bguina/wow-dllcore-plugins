#include "pch.h"

#include "../dump/WowGameDescriptors.h"
#include "../dump/WowGameOffsets.h"

#include "WowObject.h"

WowObject::WowObject(
	const uint8_t* baseAddr
) : MemoryObject(baseAddr)
{}

const uint8_t* WowObject::getDescriptor() const {
	return *(uint8_t**)(getAddress() + WowGameOffsets::WowObject::OffsetDescriptorPointer);
}

WowGuid64 WowObject::getGuid() const {
	return ((WowGuid64*)(getAddress() + WowGameOffsets::WowObject::OffsetGuid))[0];
}

WowGuid64 WowObject::getGuid2() const {
	return ((WowGuid64*)(getAddress() + WowGameOffsets::WowObject::OffsetGuid))[1];
}

//		StorageField = 0x10,//good-33526
//		ObjectType = 0x20,//good-33526
//		NextObject = 0x70,//good-33526
//		FirstObject = 0x18,//good-33526
//		LocalGUID = 0x58, //good-33526

WowObjectType WowObject::getType() const {
	return *(WowObjectType*)(getAddress() + WowGameOffsets::WowObject::OffsetType);
}

std::string WowObject::getTypeLabel() const {
	switch (getType()) {
	case WowObjectType::Object:return  "Object";
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

const WowVector3f& WowObject::getPosition() const {
	return *((WowVector3f*)(getAddress() + WowGameOffsets::WowObject::OffsetPosition));
}

float WowObject::getX() const {
	return getPosition().x;
}

float WowObject::getY() const {
	return getPosition().y;
}

float WowObject::getZ() const {
	return getPosition().z;
}

float WowObject::getFacingRadians() const {
	return *(float*)(getAddress() + WowGameOffsets::WowObject::OffsetPosition);
}

int WowObject::getFacingDegrees() const {
	const double PI = 3.141592653589793;
	return (int)(getFacingRadians() * 180 / PI);
}

// Position helpers
float WowObject::getDistanceTo(const WowObject& object) const {
	return getPosition().getDistanceTo(object.getPosition());
}

float WowObject::getFlightDistanceTo(const WowObject& object) const {
	return getPosition().getFlightDistanceTo(object.getPosition());
}

float WowObject::getFacingDegreesTo(const WowObject& object) const {
	return getPosition().getFacingDegreesTo(object.getPosition());
}

float WowObject::getFacingDeltaDegrees(const WowObject& object) const {
	return getPosition().getFacingDeltaDegrees(getFacingDegrees(), object.getPosition());
}

// protected:
void* WowObject::vtableAt(unsigned index) {
	return ((void**)getAddress())[index];
}
