#pragma once
#include "static/StaticSpellRanks.h"

class WowUnitObject;

class ISpellRotation
{
public:
	using CastSpellPredicate = bool (*)(ISpellDefinition* spell, const WowUnitObject* target);

	virtual ~ISpellRotation() = default;

	virtual void addSpell(ISpellDefinition* spell, CastSpellPredicate predicate) = 0;
	virtual ISpellDefinition* getNextSpell(const WowGame& game, const WowUnitObject& target) const = 0;
};
