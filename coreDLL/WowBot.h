#pragma once

#include <cmath>
#include <iostream>
#include <list>

#include "Debugger.h"
#include "WowGame.h"
#include "WindowController.h"
#include "pathfinder/LinearPathFinder.h"

class WowBot
{
public:
	WowBot(
		WowGame& game
	) : mGame(game)
	{
	}

	~WowBot()
	{
		mGame.getWindowController().releaseAllKeys();
	}

	void run(Debugger& dbg) {
		if (!mBotStarted) return;

		std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();

		if (self == nullptr) {
			dbg.log("null == self");
			return;
		}


		if (mPathFinder != nullptr) {

			const Vector3f& selfPosition = self->getPosition();
			Vector3f nextPosition;

			if (mPathFinder->followPathToDestination(selfPosition, nextPosition)) {
				self->moveTo(mGame, nextPosition);
			}

			if (true) {
				std::shared_ptr<WowUnitObject> someBoar = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObject::Unit);

				if (someBoar != nullptr) {
					// Say hi to boar
					const uint32_t* boarGuid = someBoar->getGuidPointer();

					self->interactWith(mGame, boarGuid);

					if (true) {
						std::stringstream ss;

						ss << "[I] canAttack " << self->canAttack(mGame, someBoar->getAddress()) << std::endl;
						ss << "[I] isFriendly " << self->isFriendly(mGame, someBoar->getAddress()) << std::endl;
						ss << "[I] facing " << self->getFacingDegrees() << std::endl;
						dbg.log(ss.str());
					}
				}
			}
		}
		else {
			// no profile has been loaded!
			dbg.log("[E] Bot started but no PathFinder loade\n");
		}
	}

	const WowGame& getGame() const {
		return mGame;
	}

	void setBotStarted(bool started) {
		if (!started) mGame.getWindowController().releaseAllKeys();

		mBotStarted = started;
	}

	void loadLinearWaypoints(const std::vector<Vector3f>& waypoints) {
		mPathFinder = std::make_unique< LinearPathFinder>(waypoints);
	}

protected:
	WowGame& mGame;
	bool mBotStarted = false;
	std::unique_ptr<IPathFinder> mPathFinder;

private:
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowBot& obj
	)
{
	out << "[WowBot]";
	// TODO print navigator state
	return out;
}
