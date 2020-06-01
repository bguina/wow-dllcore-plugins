#pragma once

#include "../IBenGameInterpretation.h"

#include "game/WowGame.h"

/*
	Extrapoles data to give an estimated result
*/
class IBenGameInterpretor {
public:
	virtual ~IBenGameInterpretor() {}

	virtual IBenGameInterpretation* interpret(const WowGame& game) = 0;
};
