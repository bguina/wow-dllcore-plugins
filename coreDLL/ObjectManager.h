#pragma once

#include <cstdint>
#include <iostream>

#include "MemoryObject.h"

class ObjectManager : public  MemoryObject
{
public:
	ObjectManager(
		const uint8_t* baseAddr
	) : MemoryObject(baseAddr)
	{}

	const uint8_t* firstObject() {
		return *(const uint8_t**)(baseAddress() + 0x18);
	}

	const uint8_t* nextObject(const uint8_t* currentObject) {
		return *(uint8_t**)(currentObject + 0x70);
	}
};

inline std::ostream& operator<<(
	std::ostream& out,
	const ObjectManager& obj
	)
{
	out << "found ObjectManager at 0x" << std::hex << (uint64_t)obj.baseAddress() << std::endl;
	return out;
}
