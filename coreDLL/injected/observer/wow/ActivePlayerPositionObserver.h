#pragma once

#include "AWowGameObserver.h"

#include "../../process/wow/WowVector3f.h"

class Client;

class ActivePlayerPositionObserver final : public AWowGameObserver {
public:
	ActivePlayerPositionObserver(Client& server, float maxDelta);

	virtual void makeCapture(const WowGame& game);
	
private:
	float mMaxDistanceDelta;
	WowVector3f mLastPosition;
};
