#pragma once

#include "../../base/ABenAgent.h"

class IBenFisher: public ABenAgent
{
public:
	virtual ~IBenFisher() = default;;

	virtual void onResume(WowGame& game) = 0;
	virtual void onEvaluate(WowGame& game) = 0;
	virtual void onPause(WowGame& game) = 0;
};
