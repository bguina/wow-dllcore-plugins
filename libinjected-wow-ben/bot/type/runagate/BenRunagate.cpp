
#include "BenRunagate.h"
#include "../../gameplay/BenRecordedGameplay.h"
#include "../../gameplay/snapshot/evaluator/BenGameSnapshotEvaluator.h"

const std::string TAG = "BenIdleAgent";

BenRunagate::BenRunagate() :
	ABenChampion(new BenRecordedGameplay<20000, 500>(new BenGameSnapshotEvaluator()), TAG, nullptr)
{
}

BenRunagate::~BenRunagate() = default;

bool BenRunagate::onEvaluatedInFight() {
	FileLogger dbg(mDbg, "onEvaluatedIdle");

	dbg << "implement me! I want to run away" << std::endl;
	return true;
}

