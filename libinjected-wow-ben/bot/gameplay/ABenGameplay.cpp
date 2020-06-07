#include "ABenGameplay.h"

#include "snapshot/BenGameSnapshot.h"

ABenGameplay::ABenGameplay(unsigned int periodMs, IBenGameSnapshotEvaluator* evaluator) :
	mPeriodMilliseconds(periodMs),
	mEvaluator(evaluator)
{
}

ABenGameplay::~ABenGameplay() = default;

std::shared_ptr<IBenGameSnapshot> ABenGameplay::takeSnapshot(const WowGame& game) const
{
	return std::make_shared<BenGameSnapshot>(game);
}

unsigned int ABenGameplay::getPeriodMilliseconds() const
{
	return mPeriodMilliseconds;
}

void ABenGameplay::setPeriodMilliseconds(const unsigned int periodMs)
{
	mPeriodMilliseconds = periodMs;
}
