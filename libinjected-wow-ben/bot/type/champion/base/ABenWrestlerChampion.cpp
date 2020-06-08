
#include "ABenWrestlerChampion.h"
#include "../../../gameplay/ABenGameRecord.h"

ABenWrestlerChampion::ABenWrestlerChampion(ABenWowGameEvaluator* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenChampion(gameplay/*new ABenGameRecord<10000, 200>(new BenGameSnapshotEvaluator())*/, tag, runagate)
{
}

ABenWrestlerChampion::~ABenWrestlerChampion() = default;

void ABenWrestlerChampion::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenWrestlerChampion::onUnitTap(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenWrestlerChampion::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
}

bool ABenWrestlerChampion::onEvaluatedInFight()
{

	return false;
}

