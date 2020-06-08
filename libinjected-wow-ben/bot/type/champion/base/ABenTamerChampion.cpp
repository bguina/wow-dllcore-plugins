
#include "ABenTamerChampion.h"
#include "../../../gameplay/ABenGameRecord.h"

ABenTamerChampion::ABenTamerChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenDistantChampion(gameplay, tag, runagate)
{
}

ABenTamerChampion::~ABenTamerChampion() = default;

void ABenTamerChampion::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenTamerChampion::onUnitTap(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenTamerChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
}

bool ABenTamerChampion::onEvaluatedInFight()
{
	return false;
}

