#pragma once

#include "../LinearSpellRotation.h"
#include "HuntersMark.h"
#include "SerpentSting.h"
#include "ArcaneShot.h"
#include "RaptorStrike.h"

class AHunterRotation : public LinearSpellRotation
{
public:
	AHunterRotation() : LinearSpellRotation()
	{
		// Hunter's mark1
		mSpells.emplace_back(std::make_unique<HuntersMark1>(),
			[](ISpellDefinition* spell, const WowUnitObject* target)
			{
				// make sure Hunter's mark is not applied
				return !target->hasAura(spell->getSpellId());
					// todo  about to expire or is lower rank
			}
		);

		// SerpentSting1
		mSpells.emplace_back(std::make_unique<SerpentSting::Rank1>(),
			[](ISpellDefinition* spell, const WowUnitObject* target)
			{
				// make sure SerpentSting1 is not applied, 
				return !target->hasAura(spell->getSpellId());
					// todo about to expire or is lower rank
			}
		);

		// ArcaneShot1
		mSpells.emplace_back(std::make_unique<ArcaneShot1>(),
			[](ISpellDefinition* spell, const WowUnitObject* target)
			{
				return true;
			}
		);

		// RaptorStrike1
		mSpells.emplace_back(std::make_unique<RaptorStrike1>(),
			[](ISpellDefinition* spell, const WowUnitObject* target)
			{
				// Raptor Strike is costly, try not to cast it
				return true;
			}
		);
	}
};