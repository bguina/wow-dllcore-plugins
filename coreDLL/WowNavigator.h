#pragma once

#include <cmath>
#include <iostream>

#include "Debugger.h"
#include "WowGame.h"
#include "WindowController.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame& game
	) : mGame(game)
	{}

	~WowNavigator()
	{
		mGame.getWindowController().clearKeyPressed();
	}

	void run(Debugger& dbg) {
		WowActivePlayerObject* self = mGame.getObjectManager().getActivePlayer();

		if (NULL == self) {
			dbg.log("null == self");

			return;
		}

		if (botStarted) {
			const Vector3f& pos = self->getPosition();
			WowUnitObject* someBoar = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObject::Unit);

			if (NULL == someBoar) {
				dbg.log("null == someBoar");
				return;
			}


			/*
			if (waypointsPath.size() > 0)
			{
				std::stringstream ss;
				ss << "waypointsPath ==  " << waypointsPath.size() << std::endl;

				ss << "waypoint 2 ==  " << waypointsPath[5] << std::endl;
				dbg.log(ss.str().c_str());
			}
			*/



			if (true) {
				// Say hi to boar
				const uint32_t* boarGuid = someBoar->getGuidPointer();

				self->interactWith(mGame, boarGuid);
			}

			if (true) {
				// Face given position
				const Vector3f& point = someBoar->getPosition();

				self->moveTo(mGame, point);

				if (true) {
					std::stringstream ss;

					ss << "canAttack " << self->canAttack(mGame, someBoar->getAddress()) << std::endl;
					ss << "isFriendly " << self->isFriendly(mGame, someBoar->getAddress()) << std::endl;
					//ss << "facing " << self->getFacingDegrees() << ", target angle is " << angle << std::endl;
					//ss << "delta " << delta << std::endl;
					dbg.log(ss.str().c_str());
				}

			}


		}
	}

	const WowGame& getGame() const {
		return mGame;
	}

	//SETTER
	void setBotStarted(bool status) {
		botStarted = status;
		if (!botStarted)
		{
			//clearKeys
			//mGame.getWindowController()
		}
	}

	//GETTER
	std::vector<Vector3f>& getWaypointsPath() {
		return waypointsPath;
	}

protected:
	WowGame& mGame;
	bool botStarted = false;
	std::vector<Vector3f> waypointsPath;

private:
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	out << "[WowNavigator]";
	// TODO print navigator state
	return out;
}
