#include "pch.h"

#include "SpellBook.h"
#include "WowGame.h"
#include "dump/WowGameOffsets.h"

using SpellBook_CastSpell = void(__fastcall) (int spellId, int itemId, const uint128_t* pGuid128, unsigned __int8 isTrade, char padArg);

SpellBook::SpellBook(
	const uint8_t* baseAddr
) :
	mPointerAddr(baseAddr)
{
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


std::list<SpellbookDescriptor>  SpellBook::listSpells() const {
	//get All spell IDS
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));

	uint32_t totalSpell = getSpellBookCount();

	std::list<SpellbookDescriptor> res;

	for (size_t i = 0; ptrArray && i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL)
			break;
		res.push_back(*entry);
	}

	return res;
}


void SpellBook::castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target) {
	//return (game.get<CastSpellT>(0xDBAC30))((void*)getAddress(), spellId, 0, 0);
	auto spellIdx = 0;

	uint32_t totalSpell = getSpellBookCount();
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));

	for (size_t i = 0; ptrArray && i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL || ((uint64_t)entry & 1))
			break;

		if (entry->id == spellId) {
			spellIdx = i;
			break;
		}
	}

	(game.get<SpellBook_CastSpell>(0xDBAC30))(spellIdx, 0, target, 0, 0);

	//return game.get<SpellbookClickSpell>(WowGameOffsets::WowSpellbook::FunctionClickSpell)(getGuidPtr(), spellId);
	//return 1;
}

uint32_t SpellBook::getSpellBookCount() const {
	return *(uint32_t*)(mPointerAddr - 0x28);
}

void SpellBook::scan() {
	//DO SOMETHING ???
}

const uint8_t* SpellBook::getBaseAddress() const {
	return mPointerAddr;
}
