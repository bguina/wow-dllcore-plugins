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

	bool isKnown(int spellId) const;
	bool isLearnt(int spellId) const;

	void castSpell(const WowGame& game, const uint32_t spellId, const uint128_t* target);
	uint64_t* petInfoFindSpellById(const WowGame& game, const uint32_t petSpellId);
	void petInfoSendPetAction(const WowGame& game, uint64_t* spell, uint128_t* target, uint32_t unknown, uint64_t* unkwown2);
	bool orderPetToAttackTarget(const WowGame& game, uint128_t* target);

private:
	const SpellbookDescriptor* getSpell(uint32_t spellId) const;
	uint32_t getSpellBookCount() const;
	const uint8_t* mPointerAddr;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const SpellBook& spellBookMgr
	)
{
	out << "[SpellBook@]" << std::endl;

	return out;
}

