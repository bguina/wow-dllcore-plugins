#pragma once

#include <cstdint>
#include <iostream>
#include <list>

#include "object/WowGuid128.h"

class WowGame;

struct SpellbookDescriptor {
	uint32_t flags;
	uint32_t id;
};

class SpellBook
{
public:
	SpellBook(const uint8_t* baseAddr);

	std::list<SpellbookDescriptor> listSpells() const;

	uint32_t getSpellBookCount() const;

	void scan();

	const uint8_t* getBaseAddress() const;
	const SpellbookDescriptor* getSpell(uint32_t spellId) const;
	void castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target);
	uint64_t* petInfoFindSpellById(const WowGame& game, const uint32_t petSpellId);
	void petInfoSendPetAction(const WowGame& game, uint64_t* spell, uint128_t* target, uint32_t unknown, uint64_t* unkwown2);
	bool orderPetToAttackTarget(const WowGame& game, uint128_t* target);

private:
	const uint8_t* mPointerAddr;

	enum SpellbookFlags : uint32_t {
		IS_LEARNT = 1,
		IS_KNOWN = 2,
	};
};

inline std::ostream& operator<<(
	std::ostream& out,
	const SpellBook& spellBookMgr
	)
{
	out << "[SpellBook@" << (void*)spellBookMgr.getBaseAddress() << ": Count = " << spellBookMgr.getSpellBookCount() << "]" << std::endl;



	if (NULL != spellBookMgr.getBaseAddress()) {
		//Add more print here for spell count // spell list ID
	}

	return out;
}

