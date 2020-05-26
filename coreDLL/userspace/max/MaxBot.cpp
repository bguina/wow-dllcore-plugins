#include "pch.h"

#include <vector>

#include "MaxBot.h"

#include "../../injectable/wow/game/WowGame.h"
#include "../../injectable/wow/game/object/WowActivePlayerObject.h"
#include "../../injectable/wow/pathfinder/LinearPathFinder.h"

const std::string TAG = "WowMaxBot";

WowMaxBot::WowMaxBot(WowGame& game) :
	BaseWowBot(game, TAG),
	mPathFinder(nullptr),
	mTargetUnit(nullptr)
{
}

WowMaxBot::~WowMaxBot() {

}

bool WowMaxBot::handleServerMessage(ClientMessage& serverMessage) {
	bool handled = false;
	mDbg << FileLogger::verbose << "handleServerMessage " << (int)serverMessage.type << FileLogger::normal << std::endl;

	switch (serverMessage.type)
	{
	case MessageType::POST_DLL_DATA_3DPATH:
		mPathFinder = std::make_unique<LinearPathFinder>(*serverMessage.waypoints);
		handled= true;
	default:
		break;
	}

	mDbg.flush();
	return handled;
}

void WowMaxBot::onResume() {
}

void WowMaxBot::onPause() {
}

void WowMaxBot::onEvaluate() {

	_logDebug();

	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileLogger::info << TAG << " running" << FileLogger::normal << std::endl;
	mDbg.flush();

	std::shared_ptr<WowUnitObject> secondSelf(mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getGuid()));

	if (secondSelf != nullptr) {

		mDbg << FileLogger::info << "self == " << (void*)self->getAddress() << " combat == " << self->isInCombat() << FileLogger::normal << std::endl;
		mDbg.flush();


		std::shared_ptr<WowUnitObject> currentTarget = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getTargetGuid());

		if (currentTarget != nullptr)
		{
			mDbg << FileLogger::info << "target address = " << (void*)currentTarget->getAddress() << " Lootable == " << currentTarget->isLootable() << FileLogger::normal << std::endl;
			mDbg.flush();
		}

		if (nullptr != mTargetUnit && (0 == mTargetUnit->getAddress() || mBlacklistedGuids.find(mTargetUnit->getGuid()) != mBlacklistedGuids.end()))
		{
			mDbg << "GUID is blacklisted, ignoring" << mTargetUnit->getGuid().upper();
			mDbg.flush();
			mTargetUnit = nullptr;
		}

		if (nullptr == mTargetUnit)
		{
			std::list<std::shared_ptr<const WowUnitObject>> allUnits = mGame.getObjectManager().allOfType<const WowUnitObject>(WowObjectType::Unit);
			std::list<std::shared_ptr<const WowUnitObject>> whilelist;
			mDbg.i("looking for a new unit to attack");
			mDbg.flush();
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
			if (distance < 100)
			{
				mDbg.i("found new target to attack");
				mDbg.flush();
				mTargetUnit = targetUnit;
			}
		}

		// Are we far away the target unit?
		if (nullptr != mTargetUnit) {
			mDbg.i("targetting some unit...");
			mDbg.flush();
			if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) > 25)
			{
				mDbg << FileLogger::info << "My position" << self->getPosition() << FileLogger::normal << std::endl;
				mDbg << FileLogger::info << "target unit " << mTargetUnit->getGuid().upper() << " still out of reach" << mTargetUnit->getPosition() << FileLogger::normal << std::endl;
				mDbg.flush();
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
				mDbg.flush();

				mGame.getWindowController()->releaseAllKeys();
				windowController->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
				windowController->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
				// move forward if approximately on the right facing
				windowController->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);

				mDbg << FileLogger::info << "moving to " << mTargetUnit->getPosition() << FileLogger::normal << std::endl;

				mDbg << FileLogger::info << " target angle is" << self->getPosition().getFacingDegreesTo(mTargetUnit->getPosition()) << " delta angle is " << self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition()) << FileLogger::normal << std::endl;
				mDbg.flush();
			}
			else {
				mDbg.i("Killed target! yay!");
				mDbg.flush();
				mGame.getWindowController()->releaseAllKeys();
				// Unit gets "killed" (blacklisted for now)


				//mGame.getSpellBookManager().clickSpell(mGame, 22723);
				if (mTargetUnit->getUnitHealth() == 0)
				{
					self->interactWith(mGame, mTargetUnit->getGuidPtr());
					mBlacklistedGuids.insert(mTargetUnit->getGuid());
					mOpeningCombat = true;
				}
				else if (mOpeningCombat)
				{
					self->interactWith(mGame, mTargetUnit->getGuidPtr());
					mGame.getSpellBook().castSpell(mGame, 1978, self->getTargetGuidPtr());
					mOpeningCombat = false;
				}
				//else if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) < 5) {
					//mGame.getSpellBookManager().castSpell(mGame, 2973, self->getTargetGuidPtr());
				//}

			}
		}
		else {
			mDbg.i("no mTargetUnit");
			mDbg.flush();
			if (mPathFinder != nullptr) {
				const Vector3f& selfPosition = self->getPosition();
				Vector3f nextPosition;

				if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
					mDbg.i("mPathFinder moving along the path");
					mDbg.flush();
					self->moveTo(mGame, nextPosition);
				}
				else {
					mDbg.i("mPathFinder could not move along :(");
					mDbg.flush();
				}

			}
			else {
				// no profile has been loaded!
				mDbg.e("no PathFinder is loaded to move around loaded\n");
				mDbg.flush();
			}
		}
	}
	else {
		mDbg.w("no WowActivePlayerObject");
		mDbg.flush();
	}

	mDbg.flush();
}

void WowMaxBot::_logDebug() const {
	BaseWowBot::_logDebug();

	// show info relative otof current pathfinder
	LinearPathFinder* pathfinder = dynamic_cast<LinearPathFinder*>(mPathFinder.get());
	if (nullptr != pathfinder) {
		size_t waypointsCount = 0;

		waypointsCount = pathfinder->getWaypointsCount();
		mDbg << "helped by LinearPathFinder with " << waypointsCount << " waypoints " << std::endl;
	}
	else mDbg << " without PathFinder" << std::endl;
}
