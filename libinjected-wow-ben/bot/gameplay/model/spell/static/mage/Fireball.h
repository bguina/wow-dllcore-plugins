#pragma once

#include "../StaticSpellRanks.h"

static constexpr int RANKS[] = { 1,2,3 };

using Fireball = StaticSpellRanks<RANKS>;

using Fireball1 = Fireball::Rank<1, 1500>;
