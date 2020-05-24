#pragma once

#include <cstdint>
#include <iostream>

class WowGame;

class SpellBookManager
{
public:
	SpellBookManager(const uint8_t* baseAddr);

	uint32_t getSpellBookCount() const;

	void scan();

	const uint8_t* getBaseAddress() const;

	uint64_t clickSpell(const WowGame& game, uint32_t spellId);

private:
	const uint8_t* mPointerAddr;
	struct SpellbookDescriptor {
		uint32_t flags;
		uint32_t id;
	};
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

