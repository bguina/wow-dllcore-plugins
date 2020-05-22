#include "pch.h"

#include "WowActivePlayerObject.h"
#include "../WowGame.h"

typedef char(__fastcall ActivePlayerCanAttack)(const uint8_t* self, const uint8_t* target, char unknown);
typedef bool(__fastcall ActivePlayerIsFriendlyWith)(const uint8_t* self, const uint8_t* target);
typedef uint64_t(__fastcall ActivePlayerInteract)(const uint32_t*);

WowActivePlayerObject::WowActivePlayerObject(const uint8_t* baseAddr) : 
	WowPlayerObject(baseAddr)
{
}

char WowActivePlayerObject::canAttack(const WowGame& game, const WowUnitObject& target) const {
	auto canAttack = game.get<ActivePlayerCanAttack>(0x8831E0);

	return canAttack(getAddress(), target.getAddress(), 0);
}

bool WowActivePlayerObject::isFriendly(const WowGame& game, const WowUnitObject& target) const {
	auto isFriendly = game.get<ActivePlayerIsFriendlyWith>(0x8A0FF0);

	return isFriendly(getAddress(), target.getAddress());
}

uint64_t  WowActivePlayerObject::interactWith(const WowGame& game, const uint32_t* targetGuid) {
	auto UnitInteract = game.get<ActivePlayerInteract>( 0xD65D60);

	return UnitInteract(targetGuid);
}
