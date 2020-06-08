#pragma once

#include "game/object/WowGuid128.h"
#include "../IBenEvaluator.h"
#include "../../../../gameplay/snapshot/IBenGameSnapshot.h"

/*
	Give a game interpretation over time from a set of previous Snapshots or some other source of data.
*/
class IBenGameExtrapolator : public IBenEvaluator<IBenGameSnapshot, WowGame> {
public:
	virtual ~IBenGameExtrapolator() = default;

	virtual IBenGameSnapshot* extrapolate(const WowGame& game, long ms) = 0;
};
