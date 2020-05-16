#pragma once

#include "WowObject.h"

class WowCorpseObject : public WowObject
{
public:
	WowCorpseObject(
		const uint8_t* baseAddr
	) : WowObject(baseAddr)
	{}

};

inline std::ostream& operator<<(
	std::ostream& out,
	const WowCorpseObject& obj
	)
{
	out << (WowObject)obj;
	return out;
}

