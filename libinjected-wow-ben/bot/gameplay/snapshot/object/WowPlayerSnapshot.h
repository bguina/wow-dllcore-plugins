#pragma once

#include "WowUnitSnapshot.h"
#include "game/world/object/WowPlayerObject.h"

class WowPlayerSnapshot : public WowUnitSnapshot
{
public:
	WowPlayerSnapshot(const WowPlayerObject& obj);
	virtual ~WowPlayerSnapshot();

};
