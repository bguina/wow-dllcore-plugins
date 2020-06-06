
#include "ABenDistantChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenDistantChampion::ABenDistantChampion(const std::string& tag, ABenAgent* runagate) :
	ABenChampion(new BenRecordedGameplay<15000, 1000>(new BenGameSnapshotEvaluator()), tag, runagate)
{
}

ABenDistantChampion::~ABenDistantChampion() = default;

void ABenDistantChampion::onUnitAggro(const WowUnitObject & object)
{
}

void ABenDistantChampion::onUnitTap(const WowUnitObject & object)
{
}

void ABenDistantChampion::onUnitKill(const WowUnitObject & object)
{
}

bool ABenDistantChampion::onEvaluatedInFight()
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

