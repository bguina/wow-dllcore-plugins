#pragma once

#include "../../base/ABen.h"

class IBenFisher: public ABen
{
public:
	virtual ~IBenFisher() {};

	virtual void onResume(WowGame& game) = 0;
	virtual void onEvaluate(WowGame& game) = 0;
	virtual void onPause(WowGame& game) = 0;

	virtual bool handleWowMessage(ServerWowMessage& cl) = 0;
};
