#pragma once

#include "IBenGameExtrapolator.h"

class ABenGameExtrapolator : public IBenGameExtrapolator {
public:
	ABenGameExtrapolator();
	virtual ~ABenGameExtrapolator();

	// like extrapolate(0, result) but cheaper computation
	virtual IBenGameInterpretation* interpret(const WowGame& game);

	virtual bool extrapolate(long ms, IBenGameInterpretation& result) = 0;

};
