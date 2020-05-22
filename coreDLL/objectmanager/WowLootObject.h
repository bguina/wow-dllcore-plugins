#pragma once

#include "WowObject.h"

class WowLootObject : public WowObject
{
public:
	WowLootObject(const uint8_t* baseAddr);
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowLootObject& obj
	)
{
	out << (WowObject)obj;
	return out;
}
