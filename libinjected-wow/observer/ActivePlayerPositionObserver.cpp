#include <sstream>

#include "ActivePlayerPositionObserver.h"
#include "../game/WowGame.h"
#include "Client.h"

ActivePlayerPositionObserver::ActivePlayerPositionObserver(Client& client, float maxDelta)
	: AWowGameObserver("ActivePlayerPositionObserver", client, 800), mMaxDistanceDelta(maxDelta)
{}

void ActivePlayerPositionObserver::makeCapture(const WowGame& game) {
	auto self = game.getObjectManager().getActivePlayer();

	if (nullptr != self) {
		auto currentPosition = self->getPosition();
		auto distanceDelta = currentPosition.getFlightDistanceTo(mLastPosition);

		if (distanceDelta >= mMaxDistanceDelta || (mLastPosition[0] == .0f && mLastPosition[1] == .0f && mLastPosition[2] == .0f)) {
			std::stringstream ss;

			ss << currentPosition[0] << ',' << currentPosition[1] << ',' << currentPosition[2];
			mClient.sendMessage(mClient.getMessageManager().builResponseInfo("position", ss.str()));
			mLastPosition = self->getPosition();
		}
	}
	else {
		// not in game?! throw exception?
		// TODO ask server to stop observing?
		return;
	}
}
