#pragma once

#include "ABenGameExtrapolator.h"

class BenGameLinearExtrapolator : public ABenGameExtrapolator {
public:
	BenGameLinearExtrapolator();
	virtual ~BenGameLinearExtrapolator();

	virtual IBenGameSnapshot* extrapolate(const WowGame& game, long ms) override;
};
