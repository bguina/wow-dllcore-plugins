#pragma once

#include "AWowGameObserver.h"
#include "../Vector3f.h"

class ActivePlayerPositionObserver : public AWowGameObserver {
public:
	ActivePlayerPositionObserver(ServerSDK& server, float maxDelta);

	virtual void makeCapture(const WowGame& game);

private:
	float mMaxDistanceDelta;
	Vector3f mLastPosition;
};
