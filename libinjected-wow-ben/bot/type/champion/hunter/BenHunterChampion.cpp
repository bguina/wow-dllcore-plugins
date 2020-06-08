#include "BenHunterChampion.h"

BenHunterChampion::BenHunterChampion(ABenWowGameEvaluator* gameplay) :
	BenHunterChampion(gameplay, "BenHunterChampion", nullptr)
{
}

BenHunterChampion::BenHunterChampion(ABenWowGameEvaluator* gameplay, const std::string& tag) :
	BenHunterChampion(gameplay, tag, nullptr)
{
}

BenHunterChampion::BenHunterChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenChampion* runagate) :
	ABenTamerChampion(gameplay, tag, runagate),
	mSpells({ 3044, 1978, 2973})
{
}

BenHunterChampion::~BenHunterChampion() = default;

float BenHunterChampion::getEngagementRange() const
{
	return 25;
}

float BenHunterChampion::getPreferredRange() const
{
	return 22;
}

bool BenHunterChampion::pull(const WowUnitObject& unit)
{
	return engage(unit);
}

bool BenHunterChampion::engage(const WowUnitObject& unit)
{
	mGame->getSpellBook().castSpell(*mGame, mSpells.front(), unit.getTargetGuidPtr());
	return true;
}

bool BenHunterChampion::isPositionSatisfying(const WowVector3f& position)
{
	return true;
}


