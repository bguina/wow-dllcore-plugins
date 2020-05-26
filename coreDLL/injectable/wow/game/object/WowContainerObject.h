#pragma once

#include "WowObject.h"

class WowContainerObject : public WowObject
{
public:
	WowContainerObject(const uint8_t* baseAddr);
};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowContainerObject& obj
	)
{
	out << (WowObject)obj;
	return out;
}

