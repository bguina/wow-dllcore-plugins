
#include "ABenTamerChampion.h"
#include "../../../gameplay/BenRecordedGameplay.h"
#include "../../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

ABenTamerChampion::ABenTamerChampion(const std::string& tag, ABenAgent* runagate) :
	ABenDistantChampion(tag, runagate)
{
}

ABenTamerChampion::~ABenTamerChampion() = default;

void ABenTamerChampion::onUnitAggro(const WowUnitObject & object)
{
}

void ABenTamerChampion::onUnitTap(const WowUnitObject & object)
{
}

void ABenTamerChampion::onUnitKill(const WowUnitObject & object)
{
}

bool ABenTamerChampion::onEvaluatedInFight()
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

