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
	out << "found ObjectManager at " << (void*)objMgr.baseAddress() << std::endl;

	// iterate ObjectManger linked list
	for (
		auto pObj = objMgr.firstObject();
		NULL != pObj && !((uint64_t)pObj & 1);
		pObj = objMgr.nextObject(pObj)
		) {
		WowObject obj(pObj);

		switch (obj.getType()) {
		case WowObject::Object: out << obj; break;
		case WowObject::Item: out << obj; break;
		case WowObject::Container: out << obj; break;
		case WowObject::Unit: out << WowUnitObject(pObj); break;
		case WowObject::Player:  out << WowUnitObject(pObj); break;
		case WowObject::ActivePlayer:   out << WowUnitObject(pObj); break;
		case WowObject::GameObject:  out << obj; break;
		case WowObject::DynamicObject:   out << obj; break;
		case WowObject::Corpse: out << obj; break;
		case WowObject::AreaTrigger:  out << obj; break;
		case WowObject::Scene:  out << obj; break;
		case WowObject::Conversation:  out << obj; break;
		case WowObject::AiGroup:   out << obj; break;
		case WowObject::Scenario:  out << obj; break;
		case WowObject::Loot:  out << obj; break;
		case WowObject::Invalid:  out << obj; break;
		default:  out << obj; break;
		}

		out << std::endl;
	}

	return out;
}
