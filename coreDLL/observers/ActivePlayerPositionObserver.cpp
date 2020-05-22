#include "pch.h"

#include <sstream>

#include "ActivePlayerPositionObserver.h"
#include "../WowGame.h"

ActivePlayerPositionObserver::ActivePlayerPositionObserver(ServerSDK& server, float maxDelta)
	: AWowGameObserver("ActivePlayerPositionObserver", server, 800), mMaxDistanceDelta(maxDelta)
{}

void ActivePlayerPositionObserver::makeCapture(const WowGame& game) {
	auto self = game.getObjectManager().getActivePlayer();

	if (nullptr != self) {
		auto currentPosition = self->getPosition();
		auto distanceDelta = currentPosition.getFlightDistanceTo(mLastPosition);

		if (distanceDelta >= mMaxDistanceDelta || mLastPosition.isOrigin()) {
			std::stringstream ss;

			ss << currentPosition.x << ',' << currentPosition.y << ',' << currentPosition.z;
			mServer.sendMessage(mServer.getMessageManager().builResponseInfo("position", ss.str()));
			mLastPosition = self->getPosition();
		}
	}
	else {
		// not in game?! throw exception?
		// TODO ask server to stop observing?
		return;
	}
}
