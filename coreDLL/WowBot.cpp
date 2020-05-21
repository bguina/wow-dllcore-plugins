#include "pch.h"

#include <vector>

#include "Debugger.h"
#include "WowGame.h"
#include "WindowController.h"
#include "objectmanager/WowActivePlayerObject.h"
#include "pathfinder/LinearPathFinder.h"

#include "WowBot.h"

WowBot::WowBot(
	WowGame& game
) : mGame(game)
{
}

WowBot::~WowBot()
{
	mGame.getWindowController().releaseAllKeys();
}

void WowBot::run() {
	if (!mBotStarted) return;

	std::shared_ptr<WowActivePlayerObject> self = mGame.getObjectManager().getActivePlayer();
	Debugger dbg("D:\\bt.log");

	if (self == nullptr) {
		dbg.log("[W] no WowActivePlayerObject");
		return;
	}

	auto allUnits = mGame.getObjectManager().allOfType<WowUnitObject>(WowObject::Unit);


	if (nullptr != mCurrentUnitTarget && blackListKilledGUID.find(mCurrentUnitTarget->getGuid()) != blackListKilledGUID.end())
	{
		mCurrentUnitTarget = nullptr;
	}
	if (nullptr == mCurrentUnitTarget)
	{
		std::shared_ptr<WowUnitObject> targetUnit;
		float distance = FLT_MAX;
		for (auto it = allUnits.begin(); it != allUnits.end(); it++)
		{
			if (blackListKilledGUID.find((*it)->getGuid()) == blackListKilledGUID.end())
			{
				float tmpDistace = self->getPosition().getDistanceTo((*it)->getPosition());
				if (tmpDistace < distance)
				{
					targetUnit = (*it);
					distance = tmpDistace;
				}
			}

		}
		if (distance < 30)
		{
			mCurrentUnitTarget = targetUnit;
		}
	}

	//Check with destination
	if (self->getPosition().getDistanceTo(mCurrentUnitTarget->getPosition()) > 5)
	{
		self->moveTo(mGame, mCurrentUnitTarget->getPosition());
	}
	else {
		//START ATTACK
		//KILL
		//LOOT
		blackListKilledGUID.insert(mCurrentUnitTarget->getGuid());
	}

	//Calcul distance between closest waypoint
	//Try to add more mob really far

	if (mCurrentUnitTarget == nullptr && mPathFinder != nullptr) {
		const Vector3f& selfPosition = self->getPosition();
		Vector3f nextPosition;

		/*
		if (mPathFinder->followPathToDestination(selfPosition, nextPosition)) {
			self->moveTo(mGame, nextPosition);
		}
		*/
		if (mPathFinder->moveAlong(selfPosition, nextPosition)) {
			self->moveTo(mGame, nextPosition);
		}

		/*
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
		*/
	}
	else {
		// no profile has been loaded!
		dbg.log("[E] Bot started but no PathFinder has been loaded\n");
	}

	dbg.flush();
}

const WowGame& WowBot::getGame() const {
	return mGame;
}

void WowBot::setBotStarted(bool started) {
	if (mBotStarted && !started) mGame.getWindowController().releaseAllKeys();

	mBotStarted = started;
}

void WowBot::loadLinearWaypoints(const std::vector<Vector3f>& waypoints) {
	mPathFinder = std::make_unique< LinearPathFinder>(waypoints);
}
