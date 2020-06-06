
#include "ABenWrestlerChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenWrestlerChampion::ABenWrestlerChampion(const std::string& tag, ABenAgent* runagate) :
	ABenChampion(new BenRecordedGameplay<10000, 200>(new BenGameSnapshotEvaluator()), tag, runagate)
{
}

ABenWrestlerChampion::~ABenWrestlerChampion() = default;

void ABenWrestlerChampion::onUnitAggro(const WowUnitObject & object)
{
}

void ABenWrestlerChampion::onUnitTap(const WowUnitObject & object)
{
}

void ABenWrestlerChampion::onUnitKill(const WowUnitObject & object)
{
}

bool ABenWrestlerChampion::onEvaluatedInFight()
{
	const auto frame = mGameplay->getLastFrame();

	if (nullptr != frame) {
	}
	else
	{
		// todo fixme
	}

	return false;
}

