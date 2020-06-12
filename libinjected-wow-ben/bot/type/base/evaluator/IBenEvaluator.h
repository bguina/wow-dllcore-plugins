#pragma once

#include "game/WowGame.h"

class IBenWowGameRecord;

template<typename Evaluated, typename Source>
class IBenEvaluator {
public:

	virtual ~IBenEvaluator() = default;
	
	virtual bool read(const Source& source) = 0;

	virtual bool evaluate() = 0;
	
	virtual const Evaluated& getResult() const = 0;
};
