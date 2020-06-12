#pragma once

#include "game/world/object/WowGuid128.h"
#include "../../../gameplay/snapshot/IBenWowGameSnapshot.h"
#include "../evaluator/IBenEvaluator.h"

/*
	Give a game interpretation over time from a set of previous Snapshots or some other source of data.
*/
class IBenGameExtrapolator : public IBenEvaluator<IBenWowGameSnapshot, WowGame> {
public:
	virtual ~IBenGameExtrapolator() = default;

	virtual IBenWowGameSnapshot* extrapolate(const WowGame& game, long ms) = 0;
};
