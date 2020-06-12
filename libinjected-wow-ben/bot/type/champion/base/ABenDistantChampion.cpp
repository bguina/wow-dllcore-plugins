
#include "ABenDistantChampion.h"
#include "../../../gameplay/AWowGameRecord.h"

ABenDistantChampion::ABenDistantChampion(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay, const std::string& tag) :
	ABenChampion(gameplay/*new AWowGameRecord<15000, 1000>(new BenGameSnapshotEvaluator())*/, tag)
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

bool ABenDistantChampion::onEvaluatedAsChampion()
{
	return onEvaluatedAsDistantChampion();
}


