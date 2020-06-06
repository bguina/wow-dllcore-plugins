#include "BenGameSnapshotEvaluator.h"

#include "../BenGameSnapshot.h"
#include "../../model/Threat.h"

BenGameSnapshotEvaluator::BenGameSnapshotEvaluator() = default;

BenGameSnapshotEvaluator::~BenGameSnapshotEvaluator() = default;

bool BenGameSnapshotEvaluator::evaluateIsAttacked(const IBenGameSnapshot & snapshot, const WowUnitObject& unit) const
{
	return false;
}

IThreat* BenGameSnapshotEvaluator::evaluateUnitThreat(const IBenGameSnapshot& snapshot, const WowUnitObject& unit) const
{
	/*auto* threat = new Threat(0);*/
	return new Threat(0);
}

IThreat* BenGameSnapshotEvaluator::evaluatePositionThreat(const IBenGameSnapshot& snapshot, const WowVector3f& position) const
{
	//auto* threat = new Threat();
	return new Threat(0);
}
