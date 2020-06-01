#pragma once

#include "game/object/WowGuid128.h"
#include "../IBenGameInterpretation.h"

class ABenGameInterpretor {
public:
	ABenGameInterpretor();
	virtual ~ABenGameInterpretor();

	virtual IBenGameInterpretation* interpret(const WowGame& game) = 0;
};
