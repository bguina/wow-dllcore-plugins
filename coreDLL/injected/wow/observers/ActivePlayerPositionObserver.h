#pragma once

#include "AWowGameObserver.h"
#include "../WowVector3f.h"

class ActivePlayerPositionObserver final : public AWowGameObserver {
public:
	ActivePlayerPositionObserver(ServerSDK& server, float maxDelta);

	virtual void makeCapture(const WowGame& game);
	
private:
	float mMaxDistanceDelta;
	WowVector3f mLastPosition;
};
