#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include "../Debugger.h"
#include "../MemoryObject.h"
#include "WowObject.h"
#include "WowActivePlayerObject.h"

/*
	- what are smart pointers?
	https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c
	- shared_ptr / weak_ptr?
	https://stackoverflow.com/a/215428/4797134

*/

class ObjectManager
{
public:
	ObjectManager(const uint8_t** baseAddr);

	bool isEnabled() const;

	void scan();

	const uint8_t* getBaseAddress() const;

	std::map<uint64_t, std::shared_ptr<WowObject>>::const_iterator begin() const;
	std::map<uint64_t, std::shared_ptr<WowObject>>::iterator begin();

	std::map<uint64_t, std::shared_ptr<WowObject>>::const_iterator end() const;
	std::map<uint64_t, std::shared_ptr<WowObject>>::iterator end();

	template<class T >
	const std::shared_ptr<const T> anyOfType(WowObject::Type type) const {
		for (auto it = begin(); it != end(); ++it) {
			if (type == it->second->getType())
				return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
	}

	template<class T >
	std::shared_ptr<T> anyOfType(WowObject::Type type) {
		return std::const_pointer_cast<T>(std::as_const(*this).anyOfType<T>(type));
	}

	template<class T >
	std::vector<const std::shared_ptr<T>>& allOfType(WowObject::Type type) const {
		std::vector<const T&> results;

		for (auto it = begin(); it != end(); ++it) {
			if (type == it->second->getType())
				results.push_back(std::static_pointer_cast<T>(it->second));
		}

		return results;
	}

	template<class T >
	std::vector<std::shared_ptr<T>>& allOfType(WowObject::Type type) {
		return const_cast<std::vector<std::shared_ptr<T>>>(allOfType<T>(type));
	}

	const std::shared_ptr<const WowActivePlayerObject> getActivePlayer() const;

	std::shared_ptr<WowActivePlayerObject> getActivePlayer();

private:
	const uint8_t** mPointerAddr;
	std::map<WowGuid64, std::shared_ptr<WowObject>> mObjects;
};

inline Debugger& operator<<(
	Debugger& dbg,
	const ObjectManager& objMgr
	)
{
	std::stringstream out;
	out << "[ObjectManager@" << (void*)objMgr.getBaseAddress() << ":" << (objMgr.isEnabled() ? "ENABLED" : "DISABLED") << "]" << std::endl;

	if (NULL != objMgr.getBaseAddress()) {
		// iterate ObjectManger linked list
		for (
			auto it = objMgr.begin();
			it != objMgr.end();
			++it
			) {
			const std::shared_ptr<WowObject> obj(it->second);

			switch (obj->getType()) {
			case WowObject::Object: out << *obj; break;
			case WowObject::Item: out << *obj; break;
			case WowObject::Container: out << *obj; break;
			case WowObject::Unit: out << obj->downcast<WowUnitObject>(); break;
			case WowObject::Player:  out << obj->downcast<WowPlayerObject>(); break;
			case WowObject::ActivePlayer:   out << obj->downcast<WowActivePlayerObject>(); break;
			case WowObject::GameObject:  out << *obj; break;
			case WowObject::DynamicObject:   out << *obj; break;
			case WowObject::Corpse: out << *obj; break;
			case WowObject::AreaTrigger:  out << *obj; break;
			case WowObject::Scene:  out << *obj; break;
			case WowObject::Conversation:  out << *obj; break;
			case WowObject::AiGroup:   out << *obj; break;
			case WowObject::Scenario:  out << *obj; break;
			case WowObject::Loot:  out << *obj; break;
			case WowObject::Invalid:  out << *obj; break;
			default:  out << *obj; break;
			}

			out << std::endl;
		}
	}

	dbg.i(out);
	return dbg;
}
