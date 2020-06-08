
#include "ABenDistantChampion.h"
#include "../../../gameplay/ABenGameRecord.h"

ABenDistantChampion::ABenDistantChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenChampion(gameplay/*new ABenGameRecord<15000, 1000>(new BenGameSnapshotEvaluator())*/, tag, runagate)
{
}

ABenDistantChampion::~ABenDistantChampion() = default;

void ABenDistantChampion::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenDistantChampion::onUnitTap(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenDistantChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
}

bool ABenDistantChampion::onEvaluatedInFight()
{
	return false;
}

