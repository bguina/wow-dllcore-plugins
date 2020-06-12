
#include "ABenWrestlerChampion.h"
#include "../../../gameplay/AWowGameRecord.h"

ABenWrestlerChampion::ABenWrestlerChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag) :
	ABenChampion(gameplay/*new AWowGameRecord<10000, 200>(new BenGameSnapshotEvaluator())*/, tag)
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

bool ABenWrestlerChampion::onEvaluatedAsChampion()
{
	return onEvaluatedAsWrestlerChampion();
}

