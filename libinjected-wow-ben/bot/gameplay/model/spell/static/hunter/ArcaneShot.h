#pragma once

#include "../StaticSpellRanks.h"

static constexpr int ARCANESHOT_SPELLIDS[] = { 3044 };

using ArcaneShots = StaticSpellRanks<ARCANESHOT_SPELLIDS>;

using ArcaneShot1 = ArcaneShots::Rank<1, 6000>;
