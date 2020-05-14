#pragma once

#include "../Vector3f.h"
#include "WowPlayerObject.h"

class WowActivePlayerObject : public WowPlayerObject
{
public:
	WowActivePlayerObject(
		const uint8_t* baseAddr
	) : WowPlayerObject(baseAddr)
	{}


};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowActivePlayerObject& obj
	)
{
	out << (WowPlayerObject)obj;
	return out;
}

