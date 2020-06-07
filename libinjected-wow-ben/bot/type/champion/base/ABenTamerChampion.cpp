
#include "ABenTamerChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenTamerChampion::ABenTamerChampion(IBenGameplay* gameplay, const std::string& tag, ABenAgent* runagate) :
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
	const auto frame = mGameplay->front();

	if (nullptr != frame) {
	}
	else
	{
		// todo fixme
	}

	return false;
}

