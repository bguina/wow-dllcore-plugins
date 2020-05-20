#include "pch.h"

#include "WowActivePlayerObject.h"
#include "../WowGame.h"

typedef char(__fastcall ActivePlayerCanAttack)(const uint8_t* self, const uint8_t* target, char unknown);
typedef bool(__fastcall ActivePlayerIsFriendlyWith)(const uint8_t* self, const uint8_t* target);

char WowActivePlayerObject::canAttack(const WowGame& game, const WowUnitObject& target) const {
	auto canAttack = game.get<ActivePlayerCanAttack>(0x8831E0);

	return canAttack(getAddress(), target.getAddress(), 0);
}

bool WowActivePlayerObject::isFriendly(const WowGame& game, const WowUnitObject& target) const {
	auto isFriendly = game.get<ActivePlayerIsFriendlyWith>(0x8A0FF0);

	return isFriendly(getAddress(), target.getAddress());
}
