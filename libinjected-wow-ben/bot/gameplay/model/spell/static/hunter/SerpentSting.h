#pragma once

#include "../StaticSpellRanks.h"

static constexpr int SERPENTSTRING_SPELLIDS[] = { 1978 };

class SerpentSting : public StaticSpellRanks<SERPENTSTRING_SPELLIDS>
{
public:
	using Rank1 = Rank<1, 1500>;

};

