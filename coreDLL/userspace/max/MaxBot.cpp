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
		handled = true;
	default:
		break;
	}

	mDbg.flush();
	return handled;
}

void WowMaxBot::onResume() {
}

void WowMaxBot::onPause() {
	mGame.getWindowController()->releaseAllKeys();
}

void WowMaxBot::onEvaluate() {

	_logDebug();

	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	mDbg << FileLogger::info << TAG << " running" << FileLogger::normal << std::endl;
	mDbg.flush();

	if (self != nullptr)
	{
		std::shared_ptr<WowUnitObject> secondSelf(mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getGuid()));

		if (secondSelf != nullptr) {

			mDbg << FileLogger::info << "self == " << (void*)self->getAddress() << " combat == " << self->isInCombat() << " Life == " << self->getUnitHealthPercentage() << FileLogger::normal << std::endl;

			/*
			Debug to find all the pet spell ID
			size_t i = -100000;
			while (i < 1000000)
			{
				uint64_t* ptrTmp = mGame.getSpellBook().petInfoFindSpellById(mGame, i);
				if (ptrTmp != nullptr)
				{
					mDbg << FileLogger::info << "[Spell Pet Attack at : " << ptrTmp << " id == " << i << FileLogger::normal << std::endl;
				}
				i++;
			}
			*/


			std::shared_ptr<WowUnitObject> currentTarget = mGame.getObjectManager().getObjectByGuid<WowUnitObject>(self->getTargetGuid());

			if (currentTarget != nullptr)
			{
				mDbg << FileLogger::info << "target address = " << (void*)currentTarget->getAddress() << " Lootable == " << currentTarget->isLootable() << FileLogger::normal << std::endl;
			}


			if (nullptr != mTargetUnit && (0 == mTargetUnit->getAddress() || mBlacklistedGuids.find(mTargetUnit->getGuid()) != mBlacklistedGuids.end()))
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
					if (self->isFriendly(mGame, *(*it)) || (abs(self->getUnitLevel() - (*it)->getUnitLevel()) >= 6) &&
						mBlacklistedGuids.find((*it)->getGuid()) == mBlacklistedGuids.end())
					{
						mBlacklistedGuids.insert((*it)->getGuid());
					}
					if (mBlacklistedGuids.find((*it)->getGuid()) == mBlacklistedGuids.end())
					{
						float unitDistance = self->getDistanceTo(**it);
						if (unitDistance < distance) {
							targetUnit = *it;
							distance = unitDistance;
						}
					}

				}
				if (distance < 50)
				{
					if (mPathFinder != nullptr) {
						const Vector3f& selfPosition = self->getPosition();
						Vector3f nextPosition;

						if (mPathFinder->moveAlong(selfPosition, nextPosition) && targetUnit->getPosition().getDistanceTo(nextPosition) < 30) {
							mTargetUnit = targetUnit;
						}
						else {
							mDbg.i("mPathFinder could not move along :(");
							mTargetUnit = targetUnit;
						}
					}
					else {
						mDbg.i("found new target to attack");
						mTargetUnit = targetUnit;
					}
				}
			}

			// Are we far away the target unit?
			if (nullptr != mTargetUnit && self->getUnitHealthPercentage() > 80) {
				mDbg.i("targetting some unit...");

				int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition());
				int anglePrecision = 10;

				if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) > 25)
				{
					mDbg << FileLogger::info << "My position" << self->getPosition() << FileLogger::normal << std::endl;
					mDbg << FileLogger::info << "target unit " << mTargetUnit->getGuid().upper() << " still out of reach" << mTargetUnit->getPosition() << FileLogger::normal << std::endl;
					//mDbg.i("target unit still out of reach");

					self->moveTo(mGame, mTargetUnit->getPosition());

					/*
					mDbg << FileLogger::warn
						<< "pressing left " << (delta > anglePrecision)
						<< "pressing forward " << (abs(delta) < anglePrecision * 2)
						<< "pressing right " << (delta < -anglePrecision)
						<< FileLogger::normal << std::endl;

					mGame.getWindowController()->releaseAllKeys();
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
					// move forward if approximately on the right facing
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);

					*/


					mDbg << FileLogger::info << "moving to " << mTargetUnit->getPosition() << FileLogger::normal << std::endl;

					mDbg << FileLogger::info << " target angle is" << self->getPosition().getFacingDegreesTo(mTargetUnit->getPosition()) << " delta angle is " << self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), mTargetUnit->getPosition()) << FileLogger::normal << std::endl;
				}
				else if (delta > anglePrecision || delta < -anglePrecision) {
					mGame.getWindowController()->releaseAllKeys();
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
				}
				/*
				else if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) >= 5 &&
					self->getPosition().getDistanceTo(mTargetUnit->getPosition()) <= 8 &&
					mTargetUnit->getTargetGuid() != self->getGuid() &&
					mTargetUnit->getUnitHealth() != 0) {
					mGame.getWindowController()->pressKey(WinVirtualKey::WVK_S, true);
				}
				*/
				else {
					mDbg.i("Killed target! yay!");
					mGame.getWindowController()->releaseAllKeys();
					// Unit gets "killed" (blacklisted for now)

					if (mTargetUnit->getUnitHealth() == 0)
					{
						toLoop++;
						if (mTargetUnit->isLootable() && toLoop < 10)
						{
							self->interactWith(mGame, mTargetUnit->getGuidPtr());
						}
						else if (toLoop == 10) {
							mBlacklistedGuids.insert(mTargetUnit->getGuid());
							mInteractWith = true;
							mOpeningCombat = true;
							cacAttack = true;
							toLoop = 0;
						}
					}
					else if (mInteractWith == false && mOpeningCombat)
					{
						mGame.getSpellBook().castSpell(mGame, 13549, mTargetUnit->getGuidPtr()); //1978 serpent sting rank 1
						mOpeningCombat = false;
					}
					else if (mInteractWith) {
						self->interactWith(mGame, mTargetUnit->getGuidPtr());
						mGame.getSpellBook().orderPetToAttackTarget(mGame, mTargetUnit->getGuidPtr());
						mInteractWith = false;
					}

					else if (self->getPosition().getDistanceTo(mTargetUnit->getPosition()) < 5 && cacAttack) {
						mGame.getSpellBook().castSpell(mGame, 14260, mTargetUnit->getGuidPtr());//2973 rank1 raptor strike
						cacAttack = false;
					}

				}
			}
			else if (self->getUnitHealthPercentage() > 80) {
				mDbg.i("no mTargetUnit");
				if (mPathFinder != nullptr) {
					const Vector3f& selfPosition = self->getPosition();
					Vector3f nextPosition;

					if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
						mDbg.i("mPathFinder moving along the path");
						self->moveTo(mGame, nextPosition);


						/*
						int delta = self->getPosition().getFacingDeltaDegrees(self->getFacingDegrees(), nextPosition);
						int anglePrecision = 10;
						mGame.getWindowController()->releaseAllKeys();
						mGame.getWindowController()->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
						mGame.getWindowController()->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
						// move forward if approximately on the right facing
						mGame.getWindowController()->pressKey(WinVirtualKey::WVK_W, abs(delta) < anglePrecision * 2);
						*/
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
	}

	else {
		mDbg.w("no WowActivePlayerObject");
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
