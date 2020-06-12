#include "WowPlayerSnapshot.h"

#include "game/world/object/WowPlayerObject.h"

WowPlayerSnapshot::WowPlayerSnapshot(const WowPlayerObject& obj) :
	WowUnitSnapshot(obj)
{
}

WowPlayerSnapshot::~WowPlayerSnapshot() = default;
