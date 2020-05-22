#include "pch.h"

#include <vector>



#include "WowBot.h"

#include "../../pathfinder/LinearPathFinder.h"
#include "../../debugger/FileDebugger.h"
#include "../WowGame.h"
#include "../objectmanager/WowActivePlayerObject.h"

WowBot::WowBot(WowGame& game) :
	AGameBot(game),
	mPaused(true),
	mPathFinder(nullptr),
	mCurrentUnitTarget(nullptr)
{
}

WowBot::~WowBot()
{
}

void WowBot::pause(bool paused) {
	mPaused = paused;
}

bool WowBot::isPaused() const {
	return mPaused;
}

//#include <algorithm>
void WowBot::run() {
	size_t waypointsCount = 0;
	auto linearWaypoints = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
	if (nullptr != linearWaypoints) {
		waypointsCount = dynamic_cast<LinearPathFinder*>(mPathFinder.get())->getWaypointsCount();
	}

	if (mPaused) {
		mDbg << FileDebugger::info << "bot not started" << FileDebugger::normal << std::endl;
	}
	else {
		mDbg << FileDebugger::info << "bot started" << FileDebugger::normal << std::endl;
	}

	mDbg << FileDebugger::info <<  waypointsCount << " waypoints" << FileDebugger::normal << std::endl;

	if (!mPaused) {
		std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();

		if (self != nullptr) {
			std::list<std::shared_ptr<const WowUnitObject>> allUnits = mGame.getObjectManager().allOfType<const WowUnitObject>(WowObjectType::Unit);
			std::list<std::shared_ptr<const WowUnitObject>> unblacklistedUnits;
			//std::copy_if(allUnits.begin(), allUnits.end(), std::back_inserter(unblacklistedUnits), [](std::shared_ptr<const WowUnitObject> u) {return blackListKilledGUID.find(u->getGuid()) != blackListKilledGUID.end()  == 0; });

			if (nullptr != mCurrentUnitTarget && mBlacklistedGuids.find(mCurrentUnitTarget->getGuid()) != mBlacklistedGuids.end())
			{
				mDbg << "GUID is blacklisted, ignoring" << mCurrentUnitTarget->getGuid();
				mCurrentUnitTarget = nullptr;
			}

			if (nullptr == mCurrentUnitTarget)
			{
				mDbg.i("looking for a new unit to attack");
				std::shared_ptr<const WowUnitObject> targetUnit(nullptr);
				float distance = FLT_MAX;

				for (auto it = allUnits.begin(); it != allUnits.end(); it++)
				{
					if (mBlacklistedGuids.find((*it)->getGuid()) == mBlacklistedGuids.end())
					{
						float unitDistance = self->getDistanceTo(**it);
						if (unitDistance < distance)
						{
							targetUnit = *it;
							distance = unitDistance;
						}
					}

				}
				if (distance < 30)
				{
					mDbg.i("found new target to attack");
					mCurrentUnitTarget = targetUnit;
				}
			}

			// Are we far away the target unit?
			if (nullptr != mCurrentUnitTarget) {
				mDbg.i("targetting some unit...");
				if (self->getPosition().getDistanceTo(mCurrentUnitTarget->getPosition()) > 5)
				{
					mDbg.i("target unit still out of reach");
					//self->moveTo(mGame, mCurrentUnitTarget->getPosition());
				}
				else {
					mDbg.i("Killed target! yay!");
					// Unit gets "killed" (blacklisted for now)
					mBlacklistedGuids.insert(mCurrentUnitTarget->getGuid());
				}
			}
			else {
				mDbg.i("no target :(");
			}

			if (mCurrentUnitTarget == nullptr && mPathFinder != nullptr) {
				const Vector3f& selfPosition = self->getPosition();
				Vector3f nextPosition;

				if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
					mDbg.i("moving along the path");
					//self->moveTo(mGame, nextPosition);
				}
				else {
					mDbg.i("could not move along :(");
				}

			}
			else {
				// no profile has been loaded!
				mDbg.e("bot started but no PathFinder has been loaded\n");
			}
		}
		else {
			mDbg.w("no WowActivePlayerObject");
		}
	}

	mDbg.flush();
}

void WowBot::loadLinearWaypoints(const std::vector<Vector3f>& waypoints) {
	mPathFinder = std::make_unique<LinearPathFinder>(waypoints);
}
