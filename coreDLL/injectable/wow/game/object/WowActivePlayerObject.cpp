#include "pch.h"

#include "WowActivePlayerObject.h"
#include "../WowGame.h"

#include "../dump/WowGameOffsets.h"

typedef char (__fastcall ActivePlayerCanAttack)(const uint8_t* self, const uint8_t* target, char unknown);
typedef bool (__fastcall ActivePlayerIsFriendlyWith)(const uint8_t* self, const uint8_t* target);
typedef uint64_t (__fastcall ActivePlayerInteract)(const WowGuid128* pTargetGuid);



typedef uint64_t(__fastcall SpellbookClickSpell)(uint32_t* spellId, const uint32_t* a);

using CastSpellT = bool(__fastcall*)(void* unit, int spellId, void* item, std::uint64_t guid);

typedef bool(__cdecl* tCastSpellById)(unsigned int SpellId, unsigned int Unk2, unsigned int Unk3, unsigned int Unk4);

// some castSpell stuff
//			only 4 xrefs to char __fastcall sub_7606E0(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, char a6, __int64 a7)	 
//			sub_7606E0 is the only xref to
//				char __fastcall sub_77D730(__int64 a1, __int64 a2, __int64 a3, _DWORD *a4, _DWORD *a5)
// 
// spell-related usercall func at
//		void __usercall sub_771AD0(signed __int64 a1@<rax>, __int64 a2@<rdx>, int *a3@<rcx>, _DWORD *a4@<r8>, unsigned int a5@<r9d>, unsigned int a6)

WowActivePlayerObject::WowActivePlayerObject(const uint8_t* baseAddr) : 
	WowPlayerObject(baseAddr)
{
}

const uint32_t* WowActivePlayerObject::getCamera(const WowGame& game) {
	return game.get<const uint32_t*>(0xD65D60);
}

char WowActivePlayerObject::canAttack(const WowGame& game, const WowUnitObject& target) const {
	return  game.get<ActivePlayerCanAttack>(0x8831E0)(getAddress(), target.getAddress(), 0);
}

bool WowActivePlayerObject::isFriendly(const WowGame& game, const WowUnitObject& target) const {
	return game.get<ActivePlayerIsFriendlyWith>(0x8A0FF0)(getAddress(), target.getAddress());
}

uint64_t  WowActivePlayerObject::interactWith(const WowGame& game, const WowGuid128* targetGuid) {
	return game.get<ActivePlayerInteract>(0xD65D60)(targetGuid);
}
