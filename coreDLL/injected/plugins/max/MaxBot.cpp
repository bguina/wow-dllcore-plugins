#include "pch.h"

#include <vector>

#include "MaxBot.h"

#include "../../process/wow/WowGame.h"
#include "../../process/wow/object/WowActivePlayerObject.h"

#include "../../../pathfinder/LinearPathFinder.h"
#include "../PluginServerMessage.h"

const std::string TAG = "WowMaxBot";

WowMaxBot::WowMaxBot(WowGame& game) :
	AWowBot(game, TAG),
	mPathFinder(nullptr),
	mTargetUnit(nullptr)
{
}

WowMaxBot::~WowMaxBot()
{
}

bool WowMaxBot::handleServerMessage(const PluginServerMessage& serverMessage) {
	if (!AWowBot::handleServerMessage(serverMessage)) {
		switch (serverMessage.type)
		{
		case MessageType::WAYPOINTS:
			mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.data.waypoints);
			return true;
		default:
			break;
		}
	}
	return false;
}

void WowMaxBot::onD3dRender() {

	AWowBot::onD3dRender();
}

void WowMaxBot::_onRunning() {
	_logDebug();

	if (!mPaused) {
		std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
		mDbg << FileLogger::info << TAG << " running" << FileLogger::normal << std::endl;

		std::shared_ptr<WowUnitObject> secondSelf(mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getGuid()));

		if (secondSelf != nullptr) {

			mDbg << FileLogger::info << "self == " << (void*)self->getAddress() << " combat == " << self->isInCombat() << FileLogger::normal << std::endl;


			std::shared_ptr<WowUnitObject> currentTarget = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getTargetGuid());

			if (currentTarget != nullptr)
			{
				mDbg << FileLogger::info << "target address = " << (void*)currentTarget->getAddress() << FileLogger::normal << std::endl;
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
					mDbg << FileLogger::info << "My position" << self->getPosition() << FileLogger::normal << std::endl;
					mDbg << FileLogger::info << "target unit " << mTargetUnit->getGuid().upper() << " still out of reach" << mTargetUnit->getPosition() << FileLogger::normal << std::endl;
					//mDbg.i("target unit still out of reach");

					//self->moveTo(mGame, mTargetUnit->getPosition());


					int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition());
					int anglePrecision = 10;

					auto windowController = mGame.getWindowController();
					mDbg << FileLogger::warn
						<< "pressing left " << (delta > anglePrecision)
						<< "pressing forward " << (abs(delta) < anglePrecision * 2)
						<< "pressing right " << (delta < -anglePrecision)
						<< FileLogger::normal << std::endl;

					mGame.getWindowController()->releaseAllKeys();
					windowController->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
					windowController->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
					// move forward if approximately on the right facing
					windowController->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);

					mDbg << FileLogger::info << "moving to " << mTargetUnit->getPosition() << FileLogger::normal << std::endl;

					mDbg << FileLogger::info << " target angle is" << self->getPosition().getFacingDegreesTo(mTargetUnit->getPosition()) << " delta angle is " << self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition()) << FileLogger::normal << std::endl;

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

void WowMaxBot::_logDebug() const {
	AWowBot::_logDebug();

	// show info relative otof current pathfinder
	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << " without PathFinder" << std::endl;
}
