#pragma once

#include <memory> // for std::allocator, std::shared_ptr
#include <cstdint>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <iostream>

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

	std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator begin() const;
	std::map<WowGuid64, std::shared_ptr<WowObject>>::const_iterator end() const;

	std::map<WowGuid64, std::shared_ptr<WowObject>>::iterator begin();
	std::map<WowGuid64, std::shared_ptr<WowObject>>::iterator end();

	template<class T >
	const std::shared_ptr<const T> getObjectByGuid(WowGuid64 guid) const {
		for (auto it = begin(); it != end(); ++it) {
			if (guid == it->second->getGuid())
				return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
	}

	template<class T >
	const std::shared_ptr<const T> anyOfType(WowObjectType type) const {
		for (auto it = begin(); it != end(); ++it) {
			if (type == it->second->getType())
				return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
	}

	template<class T>
	std::shared_ptr<T> anyOfType(WowObjectType type) {
		return std::const_pointer_cast<T>(std::as_const(*this).anyOfType<T>(type));
	}

	template<class T>
	std::list<std::shared_ptr<const T>> allOfType(WowObjectType type) const {
		std::list<std::shared_ptr<const T>> results;

		for (auto it = begin(); it != end(); ++it) {
			if (type == it->second->getType())
				results.push_back(std::static_pointer_cast<T>(it->second));
		}

		return results;
	}

	template<class T>
	std::list<std::shared_ptr<T>> allOfType(WowObjectType type) {
		std::list<std::shared_ptr<T>> results;

		for (auto it = begin(); it != end(); ++it) {
			if (type == it->second->getType())
				results.push_back(std::static_pointer_cast<T>(it->second));
		}

		return results;
	}

	const std::shared_ptr<const WowActivePlayerObject> getActivePlayer() const;

	std::shared_ptr<WowActivePlayerObject> getActivePlayer();

	size_t getObjectsCount() const;

private:
	const uint8_t** mPointerAddr;
	std::map<WowGuid64, std::shared_ptr<WowObject>> mObjects;
};

inline std::ostream& operator<<(std::ostream& out,const ObjectManager& objMgr)
{
	out << "[ObjectManager@" << (void*)objMgr.getBaseAddress() << ":" << (objMgr.isEnabled() ? "ENABLED" : "DISABLED") << " with " << objMgr.getObjectsCount() << " objects]" << std::endl;
	return out;
}
