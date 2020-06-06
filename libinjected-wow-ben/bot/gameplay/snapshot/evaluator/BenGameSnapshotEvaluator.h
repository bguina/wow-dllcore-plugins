#pragma once

#include "IBenGameSnapshotEvaluator.h"

class BenGameSnapshotEvaluator : public IBenGameSnapshotEvaluator {
public:
	BenGameSnapshotEvaluator();
	virtual ~BenGameSnapshotEvaluator();

	bool evaluateIsAttacked(const IBenGameSnapshot& snapshot, const WowUnitObject& unit) const override;
	IThreat* evaluateUnitThreat(const IBenGameSnapshot& snapshot, const WowUnitObject& unit) const override;
	IThreat* evaluatePositionThreat(const IBenGameSnapshot& snapshot, const WowVector3f& position) const override;
};
