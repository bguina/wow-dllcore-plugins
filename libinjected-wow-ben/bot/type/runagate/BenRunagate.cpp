
#include "BenRunagate.h"
#include "../../gameplay/BenGameRecord.h"
#include "../base/evaluator/BenWowGameEvaluator.h"

const std::string TAG = "BenIdleAgent";

BenRunagate::BenRunagate() :
	ABenChampion(new BenWowGameEvaluator(new BenGameRecord<20000, 500>()), TAG, nullptr)
{
}

BenRunagate::~BenRunagate() = default;

bool BenRunagate::onEvaluatedInFight() {
	FileLogger dbg(mDbg, "onEvaluatedIdle");

	dbg << "implement me! I want to run away" << std::endl;
	return true;
}

