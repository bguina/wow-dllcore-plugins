
#include "LinearSpellRotation.h"

#include "StaticSpellRanks.h"

void LinearSpellRotation::addSpell(ISpellDefinition* spell, CastSpellPredicate predicate)
{
	mSpells.emplace_back(std::unique_ptr<ISpellDefinition>(spell), predicate);
}

ISpellDefinition* LinearSpellRotation::getNextSpell(const WowGame& game, const WowUnitObject& target) const
{
	const auto& firstSpell = std::find_if(mSpells.begin(), mSpells.end(), 
		[&](const std::pair<std::unique_ptr<ISpellDefinition>, CastSpellPredicate>& v)->bool
	{
			return !v.first->isOnCooldown(game) && v.second(v.first.get(), &target);
	});
	
	if (mSpells.end() == firstSpell)  
		return nullptr; // no spell found

	return firstSpell->first.get();
}
