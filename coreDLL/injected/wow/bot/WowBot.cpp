#include "pch.h"

#include <vector>

#include "WowBot.h"

#include "../../../pathfinder/LinearPathFinder.h"
#include "../../../debugger/FileDebugger.h"
#include "../WowGame.h"
#include "../objectmanager/WowActivePlayerObject.h"

WowBot::WowBot(WowGame& game) :
	mGame(game),
	mDbg("WowBotWowBot"),
	mPaused(true),
	mPathFinder(nullptr),
	mDirectUnitAttack(nullptr)
{
}

WowBot::~WowBot()
{
}

void WowBot::pause(bool paused) {
	mPaused = paused;
	if (paused)
	{
		mGame.getWindowController()->releaseAllKeys();
	}

	mDbg << FileDebugger::info << "WowBot " << (mPaused ? "paused" : "running") << FileDebugger::normal << std::endl;
}

bool WowBot::isPaused() const {
	return mPaused;
}

void WowBot::flushDebug() {
	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileDebugger::info;

	if (nullptr != self) {
		// show info relative to self
		mDbg << "position is " << self->getPosition() << " angle is " << self->getFacingDegrees() << "target unit is " << self->getTargetGuid() << std::endl;

		// show info relative otof current pathfinder
		LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
		if (nullptr != pathfinder) {
			size_t waypointsCount = 0;

			waypointsCount = pathfinder->getWaypointsCount();
			mDbg << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
		}
		else mDbg << " without PathFinder" << std::endl;

		// show info relative to a random unit around, if any
		std::shared_ptr<WowUnitObject> any = mGame.getObjectManager().anyOfType<WowUnitObject>(WowObjectType::Unit);
		if (nullptr != any) {
			mDbg << "anyunit position is " << any->getPosition()
				<< "facing it requires angle of "
				<< self->getFacingDegreesTo(*any)
				<< "angle delta is " << self->getFacingDeltaDegrees(*any) << std::endl;
		}
	}
	mDbg << FileDebugger::normal;

}

void WowBot::run() {


	if (!mPaused) {
		std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
		mDbg << FileDebugger::info << "angle is " << self->getFacingDegrees() << FileDebugger::normal << std::endl;

		mDbg << FileDebugger::info << "WowBot running" << FileDebugger::normal << std::endl;

		if (self != nullptr) {


			if (nullptr != mDirectUnitAttack && (mBlacklistedGuids.find(mDirectUnitAttack->getGuid()) != mBlacklistedGuids.end()))
			{
				mDbg << "GUID is blacklisted, ignoring" << mDirectUnitAttack->getGuid();
				mDirectUnitAttack = nullptr;
			}

			if (nullptr == mDirectUnitAttack)
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
				if (distance < 5000)
				{
					mDbg.i("found new target to attack");
					mDirectUnitAttack = targetUnit;
				}
			}

			// Are we far away the target unit?
			if (nullptr != mDirectUnitAttack) {
				mDbg.i("targetting some unit...");
				if (self->getPosition().getDistanceTo(mDirectUnitAttack->getPosition()) > 5)
				{
					mDbg << FileDebugger::info << "My position" << self->getPosition() << FileDebugger::normal << std::endl;
					mDbg << FileDebugger::info << "target unit " << mDirectUnitAttack->getGuid() << " still out of reach" << mDirectUnitAttack->getPosition() << FileDebugger::normal << std::endl;
					//mDbg.i("target unit still out of reach");

					//self->moveTo(mGame, mDirectUnitAttack->getPosition());


					int angle = self->getPosition().getFacingDegreesTo(mDirectUnitAttack->getPosition());
					int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mDirectUnitAttack->getPosition());
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

					mDbg << FileDebugger::info << "moving to " << mDirectUnitAttack->getPosition() << FileDebugger::normal << std::endl;

					mDbg << FileDebugger::info << " target angle is" << self->getPosition().getFacingDegreesTo(mDirectUnitAttack->getPosition()) << " delta angle is " << self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mDirectUnitAttack->getPosition()) << FileDebugger::normal << std::endl;

				}
				else {
					mDbg.i("Killed target! yay!");
					// Unit gets "killed" (blacklisted for now)
					mBlacklistedGuids.insert(mDirectUnitAttack->getGuid());


				}
			}
			else {
				mDbg.i("no mDirectUnitAttack");
				if (mPathFinder != nullptr) {
					const Vector3f& selfPosition = self->getPosition();
					Vector3f nextPosition;

					if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
						mDbg.i("mPathFinder moving along the path");
						//self->moveTo(mGame, nextPosition);
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

void WowBot::loadLinearWaypoints(const std::vector<Vector3f>& waypoints) {
	mPathFinder = std::make_unique<LinearPathFinder>(waypoints);
}
