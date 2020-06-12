#pragma once
#include <list>
#include <memory>


#include "../ISpellRotation.h"

class ISpellDefinition;
class WowUnitObject;

class LinearSpellRotation : ISpellRotation
{
public:

	virtual ~LinearSpellRotation() = default;

	void addSpell(ISpellDefinition* spell, CastSpellPredicate predicate) override;
	ISpellDefinition* getNextSpell(const WowGame& game, const WowUnitObject& target) const override;

protected:
	std::list<std::pair<std::unique_ptr<ISpellDefinition>, CastSpellPredicate>> mSpells;
};
