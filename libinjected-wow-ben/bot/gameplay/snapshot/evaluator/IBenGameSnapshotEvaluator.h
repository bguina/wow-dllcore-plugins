#pragma once

#include "../IBenGameSnapshot.h"
#include "../../model/IThreat.h"
#include "game/WowGame.h"

class IBenGameSnapshotEvaluator {
public:
	virtual ~IBenGameSnapshotEvaluator() = default;

	virtual bool evaluateIsAttacked(const IBenGameSnapshot& snapshot, const WowUnitObject& unit) const = 0;
	virtual IThreat* evaluatePositionThreat(const IBenGameSnapshot& snapshot, const WowVector3f& position) const = 0;
	virtual IThreat* evaluateUnitThreat(const IBenGameSnapshot& snapshot, const WowUnitObject& unit) const = 0;
};
