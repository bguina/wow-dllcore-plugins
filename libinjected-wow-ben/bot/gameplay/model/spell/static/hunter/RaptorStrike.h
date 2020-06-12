#pragma once

#include "../StaticSpellRanks.h"

static constexpr int RAPTORSTRIKE_SPELLIDS[] = { 2973 };

using RaptorStrikes = StaticSpellRanks<RAPTORSTRIKE_SPELLIDS>;

using RaptorStrike1 = RaptorStrikes::Rank<1, 6000>;
