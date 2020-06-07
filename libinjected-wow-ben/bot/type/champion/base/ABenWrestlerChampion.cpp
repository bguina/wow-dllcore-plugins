
#include "ABenWrestlerChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenWrestlerChampion::ABenWrestlerChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate) :
	ABenChampion(gameplay/*new BenRecordedGameplay<10000, 200>(new BenGameSnapshotEvaluator())*/, tag, runagate)
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
	const auto frame = mGameplay->front();

	if (nullptr != frame) {
	}
	else
	{
		// todo fixme
	}

	return false;
}

