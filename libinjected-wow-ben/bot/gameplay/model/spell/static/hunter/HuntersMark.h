#pragma once

#include "../StaticSpellRanks.h"

static constexpr int HUNTERSMARKS_SPELLIDS[] = { 1130 };

using HuntersMarks = StaticSpellRanks<HUNTERSMARKS_SPELLIDS>;

using HuntersMark1 = HuntersMarks::Rank<1, 1500>;
