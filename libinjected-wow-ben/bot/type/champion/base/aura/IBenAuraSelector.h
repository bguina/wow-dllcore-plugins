#pragma once

#include "game/WowGame.h"

class IBenWowGameRecord;

class IBenAuraSelector {
public:

	virtual ~IBenAuraSelector() = default;
	
	virtual bool select() = 0;

	virtual bool evaluate() = 0;
	
};
