#pragma once

#include "WowObject.h"

class WowItemObject : public WowObject
{
public:
	WowItemObject(
		const uint8_t* baseAddr
	) : WowObject(baseAddr)
	{}

};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowItemObject& obj
	)
{
	out << (WowObject)obj;
	return out;
}

