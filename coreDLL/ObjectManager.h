#pragma once

#include <cstdint>
#include <iostream>

#include "MemoryObject.h"
#include "WowObject.h"
#include "WowUnitObject.h"

class ObjectManager : public  MemoryObject
{
public:
	ObjectManager(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr)
	{}

	const uint8_t* firstObject() const {
		if (baseAddress() == NULL) return NULL;

		return *(const uint8_t**)(baseAddress() + 0x18);
	}

	const uint8_t* nextObject(const uint8_t* currentObject) const {
		return *(uint8_t**)(currentObject + 0x70);
	}
};

inline std::ostream& operator<<(
	std::ostream& out,
	const ObjectManager& objMgr
	)
{
	out << "found ObjectManager at 0x" << std::hex << (uint64_t)objMgr.baseAddress() << std::endl;

	// iterate ObjectManger linked list
	for (
		auto pObj = objMgr.firstObject();
		NULL != pObj && !((uint64_t)pObj & 1);
		pObj = objMgr.nextObject(pObj)
		) {
		WowObject obj(pObj);

		switch (obj.getType()) {
		case WowObject::Object: out << obj;
		case WowObject::Item: out << obj;
		case WowObject::Container: out << obj;
		case WowObject::Unit: out << WowUnitObject(pObj);
		case WowObject::Player:  out << obj;
		case WowObject::ActivePlayer:   out << obj;
		case WowObject::GameObject:  out << obj;
		case WowObject::DynamicObject:   out << obj;
		case WowObject::Corpse: out << obj;
		case WowObject::AreaTrigger:  out << obj;
		case WowObject::Scene:  out << obj;
		case WowObject::Conversation:  out << obj;
		case WowObject::AiGroup:   out << obj;
		case WowObject::Scenario:  out << obj;
		case WowObject::Loot:  out << obj;
		case WowObject::Invalid:  out << obj;
		default:  out << obj;
		}
	}

	return out;
}
