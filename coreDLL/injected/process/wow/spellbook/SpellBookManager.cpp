#include "pch.h"

#include "SpellBookManager.h"
#include "../WowGame.h"
#include "../dump/WowGameOffsets.h"

SpellBookManager::SpellBookManager(
	const uint8_t* baseAddr
) :
	mPointerAddr(baseAddr)
{
}

std::list<SpellbookDescriptor>  SpellBookManager::listSpells() const {
	//get All spell IDS
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));
	uint32_t totalSpell = getSpellBookCount();
	std::list<SpellbookDescriptor> res;

	for (size_t i = 0; i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL)
			break;
		res.push_back(*entry);
	}

	return res;
}

const SpellbookDescriptor* SpellBookManager::getSpell(uint32_t spellId) const {

	//get All spell IDS
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));
	uint32_t totalSpell = getSpellBookCount();

	for (size_t i = 0; i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL)
			break;

		if (entry->id == spellId)
			return (SpellbookDescriptor*)entry;
	}

	return NULL;
}

//SpellBook_CastSpell = 0xDBAC30,

using SpellBook_CastSpell = void (__fastcall) (int spellId, int itemId, const uint128_t* pGuid128, unsigned __int8 isTrade, char padArg);

void SpellBookManager::castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target) {
	//return (game.get<CastSpellT>(0xDBAC30))((void*)getAddress(), spellId, 0, 0);
	auto spellIdx = 0;

		uint32_t totalSpell = getSpellBookCount();
		SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));

	for (size_t i = 0; i < totalSpell; i++) {
		SpellbookDescriptor* entry = ptrArray[i];

		if (entry == NULL)
			break;

		if (entry->id == spellId) {
			spellIdx = i;
			break;
		}
	}

	(game.get<SpellBook_CastSpell>(0xDBAC30))(spellIdx,0, target, 0, 0);

	//return game.get<SpellbookClickSpell>(WowGameOffsets::WowSpellbook::FunctionClickSpell)(getGuidPtr(), spellId);
	//return 1;
}

uint32_t SpellBookManager::getSpellBookCount() const {
	return *(uint32_t*)(mPointerAddr - 0x28);
}

void SpellBookManager::scan() {
	//DO SOMETHING ???
}

const uint8_t* SpellBookManager::getBaseAddress() const {
	return mPointerAddr;
}
