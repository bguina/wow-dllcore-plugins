#include "pch.h"

#include <vector>

#include "MaxBot.h"
#include "../../WowGame.h"

#include "../../../../pathfinder/LinearPathFinder.h"
#include "../../objectmanager/WowActivePlayerObject.h"

const std::string TAG = "WowMaxBot";

WowMaxBot::WowMaxBot(WowGame& game) : 
	WowBot(game, TAG),
	mPathFinder(nullptr),
	mTargetUnit(nullptr)
{
}

WowMaxBot::~WowMaxBot()
{
}

void WowMaxBot::logDebug() const {
	WowBot::logDebug();

	// show info relative otof current pathfinder
	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << " without PathFinder" << std::endl;
}


void WowMaxBot::run() {

	if (true) {
		logDebug();
	}

	if (!mPaused) {
		std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
		mDbg << FileDebugger::info << TAG << " running" << FileDebugger::normal << std::endl;

		std::shared_ptr<WowUnitObject> secondSelf(mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getGuid()));

		if (secondSelf != nullptr) {

			mDbg << FileDebugger::info << "self == " << (void*)self->getAddress() << " combat == " << self->isInCombat() << FileDebugger::normal << std::endl;


			std::shared_ptr<WowUnitObject> currentTarget = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getTargetGuid());

			if (currentTarget != nullptr)
			{
				mDbg << FileDebugger::info << "target address = " << (void*)currentTarget->getAddress() << FileDebugger::normal << std::endl;
			}

			if (nullptr != mTargetUnit && (mBlacklistedGuids.find(mTargetUnit->getGuid()) != mBlacklistedGuids.end()))
			{
				mDbg << "GUID is blacklisted, ignoring" << mTargetUnit->getGuid().upper();
				mTargetUnit = nullptr;
			}

			if (nullptr == mTargetUnit)
			{
				std::list<std::shared_ptr<const WowUnitObject>> allUnits = mGame.getObjectManager().allOfType<const WowUnitObject>(WowObjectType::Unit);
				std::list<std::shared_ptr<const WowUnitObject>> whilelist;
				mDbg.i("looking for a new unit to attack");
				std::shared_ptr<const WowUnitObject> targetUnit(nullptr);
				float distance = FLT_MAX;

				for (auto it = allUnits.begin(); it != allUnits.end(); it++)
				{
					if (mBlacklistedGuids.find((*it)->getGuid()) == mBlacklistedGuids.end())
					{
						float unitDistance = self->getDistanceTo(**it);
						if (unitDistance < distance) {
							targetUnit = *it;
							distance = unitDistance;
						}
					}

				}
				if (distance < 10)
				{
					mDbg.i("found new target to attack");
					mTargetUnit = targetUnit;
				}
			}

			// Are we far away the target unit?
			if (nullptr != mTargetUnit) {
				mDbg.i("targetting some unit...");
				if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) > 5)
				{
					mDbg << FileDebugger::info << "My position" << self->getPosition() << FileDebugger::normal << std::endl;
					mDbg << FileDebugger::info << "target unit " << mTargetUnit->getGuid().upper() << " still out of reach" << mTargetUnit->getPosition() << FileDebugger::normal << std::endl;
					//mDbg.i("target unit still out of reach");

					//self->moveTo(mGame, mTargetUnit->getPosition());


					int angle = self->getPosition().getFacingDegreesTo(mTargetUnit->getPosition());
					int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition());
					int anglePrecision = 10;

					auto windowController = mGame.getWindowController();
					mDbg << FileDebugger::warn
						<< "pressing left " << (delta > anglePrecision)
						<< "pressing forward " << (abs(delta) < anglePrecision * 2)
						<< "pressing right " << (delta < -anglePrecision)
						<< FileDebugger::normal << std::endl;

					mGame.getWindowController()->releaseAllKeys();
					windowController->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
					windowController->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
					// move forward if approximately on the right facing
					windowController->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);

					mDbg << FileDebugger::info << "moving to " << mTargetUnit->getPosition() << FileDebugger::normal << std::endl;

					mDbg << FileDebugger::info << " target angle is" << self->getPosition().getFacingDegreesTo(mTargetUnit->getPosition()) << " delta angle is " << self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition()) << FileDebugger::normal << std::endl;

				}
				else {
					mDbg.i("Killed target! yay!");
					// Unit gets "killed" (blacklisted for now)
					mBlacklistedGuids.insert(mTargetUnit->getGuid());


				}
			}
			else {
				mDbg.i("no mTargetUnit");
				if (mPathFinder != nullptr) {
					const Vector3f& selfPosition = self->getPosition();
					Vector3f nextPosition;

					if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
						mDbg.i("mPathFinder moving along the path");
						self->moveTo(mGame, nextPosition);
					}
					else {
						mDbg.i("mPathFinder could not move along :(");
					}

				}
				else {
					// no profile has been loaded!
					mDbg.e("no PathFinder is loaded to move around loaded\n");
				}
			}
		}
		else {
			mDbg.w("no WowActivePlayerObject");
		}
	}

	mDbg.flush();
}

void WowMaxBot::loadPathWaypoints(const std::vector<Vector3f>& waypoints) {
	mPathFinder = std::make_unique<LinearPathFinder>(waypoints);
}
