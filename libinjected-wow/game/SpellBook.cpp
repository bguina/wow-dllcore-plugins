#include "pch.h"

#include "SpellBook.h"
#include "WowGame.h"
#include "dump/WowGameOffsets.h"

SpellBook::SpellBook(
	const uint8_t* baseAddr
) :
	mPointerAddr(baseAddr)
{
}

bool SpellBook::isKnown(int spellId) const {
	auto spell = getSpell(spellId);

	return spell->flags & spell->flags & SpellbookSpellFlags::Isknown;
}

bool SpellBook::isLearnt(int spellId) const {
	auto spell = getSpell(spellId);

	return (nullptr != spell && spell->flags & SpellbookSpellFlags::IsLearnt);
}

void SpellBook::castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target) {
	//return (game.get<CastSpellT>(0xDBAC30))((void*)getAddress(), spellId, 0, 0);
	auto spellIdx = 0;

	//uint32_t totalSpell = 99999999999999  
	uint32_t totalSpell = getSpellBookCount(); // fixme really needed?
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));

	for (uint32_t i = 0; ptrArray && i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL || ((uint64_t)entry & 1))
			break;

		if (entry->id == spellId) {
			spellIdx = i;
			break;
		}
	}

	using SpellBook_CastSpell = void(__fastcall) (int spellId, int itemId, const uint128_t* pGuid128, unsigned __int8 isTrade, char padArg);
	(game.getFunction<SpellBook_CastSpell>(0xDBAC30))(spellIdx, 0, target, 0, 0);
}

const SpellbookDescriptor* SpellBook::getSpell(uint32_t spellId) const {
	//get All spell IDS
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));
	uint32_t totalSpell = getSpellBookCount();

	for (size_t i = 0; ptrArray && i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL || ((uint64_t)entry & 1))
			break;

		if (entry->id == spellId)
			return (SpellbookDescriptor*)entry;
	}

	return NULL;
}

//__int64* __fastcall sub_F03B20(int a1)
typedef uint64_t* (__fastcall PetInfo_FindSpellById)(uint32_t spellId);

uint64_t* SpellBook::petInfoFindSpellById(const WowGame& game, const uint32_t petSpellId) {
	return (game.getFunction<PetInfo_FindSpellById>(0xF03B20))(petSpellId);
}

//void __fastcall sub_F05920(int* a1, __int64 GUID, int 0, __int64 ptr -=> 0)
typedef void(__fastcall PetInfo_SendPetAction)(uint64_t* spell, uint128_t* target, uint32_t unknown, uint64_t* unkwown2);
void SpellBook::petInfoSendPetAction(const WowGame& game, uint64_t* spell, uint128_t* target, uint32_t unknown, uint64_t* unkwown2) {
	return (game.getFunction<PetInfo_SendPetAction>(0xF05920))(spell, target, unknown, unkwown2);
}

//This function is a tool to find the Pet Attack Spell
// - Search spell by 0 -> Get the ptr of the end of the pet spellbook array
// - Iterate back by 0x04 until we can find the spell id 2 which match with the Pet Attack (store as a short at the beginning)
bool SpellBook::orderPetToAttackTarget(const WowGame& game, uint128_t* target) {
	uint64_t* ptrTmp = petInfoFindSpellById(game, 0);
	uint128_t tmp = 0;

	int i = 0;
	bool found = false;
	while (i < 10 && *(uint16_t*)ptrTmp != 2) {
		ptrTmp = (uint64_t*)((uint8_t*)ptrTmp - 0x04);
		found = true;
	}
	//mDbg << FileLogger::info << "[Spell Pet Attack at : " << ptrTmp << " id == " << 0 << FileLogger::normal << std::endl;
	//mDbg << FileLogger::info << "[Spell Pet Attack at : " << ptrTmp - 0x14 << " id == " << 0 << FileLogger::normal << std::endl;
	if (found)
	{
		petInfoSendPetAction(game, ptrTmp, target, 0, (uint64_t*)&tmp);
	}
	return found;

}


uint32_t SpellBook::getSpellBookCount() const {
	return *(uint32_t*)(mPointerAddr - 0x28);
}
