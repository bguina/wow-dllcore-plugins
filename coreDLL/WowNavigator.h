#pragma once

#include <cmath>
#include <iostream>
#include <list>

#include "Debugger.h"
#include "WowGame.h"
#include "WindowController.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame& game
	) : mGame(game), mNextWaypoint(mWaypointsPath.end())
	{
	}

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

		if (mBotStarted) {
			const Vector3f& pos = self->getPosition();
			WowUnitObject* someBoar = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObject::Unit);

			if (NULL == someBoar) {
				dbg.log("null == someBoar");
				return;
			}



			if (mWaypointsPath.size() > 0)
			{
				std::stringstream ss;
				ss << "waypointsPath ==  " << mWaypointsPath.size() << std::endl;


				if (mNextWaypoint != mWaypointsPath.end())
				{
					if (self->getPosition().getDistanceTo(*mNextWaypoint) < 3)
					{
						mNextWaypoint++;
						ss << "Go to next Waypoint ==  " << std::distance(mWaypointsPath.begin(), mNextWaypoint) << std::endl;
					}
					else {
						self->moveTo(mGame, *mNextWaypoint);
						//ss << "moveTo ==  " << *mNextWaypoint << std::endl;
					}
				}
				else {
					float distance = FLT_MAX;
					for (std::list<Vector3f>::iterator it = mWaypointsPath.begin(); it != mWaypointsPath.end(); it++)
					{
						float distanceTmp = it->getDistanceTo(self->getPosition());
						if (distanceTmp < distance)
						{
							distance = distanceTmp;
							mNextWaypoint = it;
							self->moveTo(mGame, *it);
						}
					}
				}

				dbg.log(ss.str().c_str());

			}



			/*
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
			*/


		}
	}

	const WowGame& getGame() const {
		return mGame;
	}

	//SETTER
	void setBotStarted(bool status) {
		mBotStarted = status;
		if (!mBotStarted)
		{
			//clearKeys
			//mGame.getWindowController()
		}
	}

	void setWaypointsProfile(const std::vector<Vector3f>& waypoints) {
		mWaypointsPath.clear();
		mWaypointsPath.assign(waypoints.begin(), waypoints.end());
		mNextWaypoint = mWaypointsPath.end();
	}

protected:
	WowGame& mGame;
	bool mBotStarted = false;
	std::list<Vector3f> mWaypointsPath;
	std::list<Vector3f>::iterator mNextWaypoint;

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
