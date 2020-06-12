#pragma once

#include "../StaticSpellRanks.h"

static constexpr int CONCUSSIVESHOT_SPELLIDS[] = { 5116 };

using ConcussiveShot = StaticSpellRanks<CONCUSSIVESHOT_SPELLIDS>;

using ConcussiveShot1 = ConcussiveShot::Rank<1, 12000>;
