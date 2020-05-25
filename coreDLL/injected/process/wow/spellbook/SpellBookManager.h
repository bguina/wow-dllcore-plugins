#pragma once

#include <cstdint>
#include <iostream>
#include <list>

class WowGame;
#include "../object/WowGuid128.h"

struct SpellbookDescriptor {
	uint32_t flags;
	uint32_t id;
};

class SpellBookManager
{
public:
	SpellBookManager(const uint8_t* baseAddr);

	std::list<SpellbookDescriptor>  listSpells() const;
	uint32_t getSpellBookCount() const;

	void scan();

	const uint8_t* getBaseAddress() const;
	const SpellbookDescriptor* getSpell(uint32_t spellId) const;
	void castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target);

private:
	const uint8_t* mPointerAddr;

	enum SpellbookFlags : uint32_t {
		IS_LEARNT = 1,
		IS_KNOWN = 2,
	};
};

inline std::ostream& operator<<(
	std::ostream& out,
	const SpellBookManager& spellBookMgr
	)
{
	out << "[SpellBookManager@" << (void*)spellBookMgr.getBaseAddress() << ": Count = " << spellBookMgr.getSpellBookCount() << "]" << std::endl;

	if (NULL != spellBookMgr.getBaseAddress()) {
		//Add more print here for spell count // spell list ID
	}

	return out;
}

