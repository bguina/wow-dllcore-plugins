#pragma once

#include "IBenGameplay.h"
#include "snapshot/evaluator/IBenGameSnapshotEvaluator.h"

class ABenGameplay : public IBenGameplay {
public:
	ABenGameplay(unsigned int periodMs, IBenGameSnapshotEvaluator* evaluator);
	virtual ~ABenGameplay();

	std::shared_ptr<IBenGameSnapshot> takeSnapshot(const WowGame& game) const;

	unsigned int getPeriodMilliseconds() const override;
	void setPeriodMilliseconds(unsigned int periodMs) override;
	
protected:
	unsigned int mPeriodMilliseconds;
	std::unique_ptr<IBenGameSnapshotEvaluator> mEvaluator;
};
