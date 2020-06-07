
#include "ABenDistantChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenDistantChampion::ABenDistantChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenChampion(gameplay/*new BenRecordedGameplay<15000, 1000>(new BenGameSnapshotEvaluator())*/, tag, runagate)
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

