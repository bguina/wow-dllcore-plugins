#pragma once

#include "game/object/WowGuid128.h"
#include "../IBenGameInterpretor.h"

class IBenGameExtrapolator : public IBenGameInterpretor {
public:
	virtual ~IBenGameExtrapolator() {}

	virtual bool extrapolate(long ms, IBenGameInterpretation& result) = 0;

};
