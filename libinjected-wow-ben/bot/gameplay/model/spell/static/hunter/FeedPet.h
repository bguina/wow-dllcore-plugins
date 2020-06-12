#pragma once

#include "../StaticSpellRanks.h"

static constexpr int FEEDPET_SPELLIDS[] = { 1978 };

class FeedPet : public StaticSpellRanks<FEEDPET_SPELLIDS>
{
public:
	using Rank1 = Rank<1, 1500>;
};
