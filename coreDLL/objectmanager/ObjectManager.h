#pragma once

#include <cstdint>
#include <iostream>

#include "../MemoryObject.h"
#include "./WowObject.h"
#include "./WowUnitObject.h"

class ObjectManager
{
public:
	ObjectManager(
		const uint8_t** baseAddr
	) : mPointerAddr(baseAddr)
	{}

	const uint8_t* getBaseAddress() const {
		return *mPointerAddr;
	}

	const uint8_t* firstObject() const {
		if (NULL == getBaseAddress()) return NULL;

		return *(const uint8_t**)(getBaseAddress() + 0x18);
	}

	const uint8_t* nextObject(const uint8_t* currentObject) const {
		uint8_t* next = *(uint8_t**)(currentObject + 0x70);

		if (((uint64_t)next & 1)) return NULL;
		return next;
	}

	const uint8_t* getSelf() const {
		if (getBaseAddress() == NULL) return NULL;

		for (
			auto pObj = firstObject();
			NULL != pObj;
			pObj = nextObject(pObj)
			) {
			WowObject obj(pObj);

			if (obj.getType() == WowObject::ActivePlayer)
				return pObj;
		}
		return NULL;
	}

	const uint8_t* getSomeBoar() const {
		if (getBaseAddress() == NULL) return NULL;

		for (
			auto pObj = firstObject();
			NULL != pObj;
			pObj = nextObject(pObj)
			) {
			WowObject obj(pObj);

			if (obj.getType() == WowObject::Unit)
				return pObj;
		}
		return NULL;
	}

private:
	const uint8_t** mPointerAddr;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const ObjectManager& objMgr
	)
{
	out << "[ObjectManager@" << (void*)objMgr.getBaseAddress() << "]" << std::endl;

	if (objMgr.getBaseAddress() == NULL) return out;

	// iterate ObjectManger linked list
	for (
		auto pObj = objMgr.firstObject();
		NULL != pObj;
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
