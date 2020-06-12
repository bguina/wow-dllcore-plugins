
#include "BenRunagate.h"
#include "../../gameplay/BenGameRecord.h"
#include "../base/evaluator/WowGameBasicEvaluator.h"

const std::string TAG = "BenIdleAgent";

BenRunagate::BenRunagate() :
	BenRunagate(std::make_shared<WowGameBasicEvaluator>(new BenGameRecord<2, 1>()))
{
}

BenRunagate::BenRunagate(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay) :
	ABenChampion(gameplay, TAG)
{
}

BenRunagate::~BenRunagate() = default;

bool BenRunagate::onEvaluatedAsChampion() {
	FileLogger dbg(mLog, "onEvaluatedIdle");

	dbg << "implement me! I want to run away" << std::endl;
	return true;
}

