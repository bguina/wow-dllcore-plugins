#include "ABenAgent.h"

#include <algorithm>

#include "game/WowGame.h"
#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"

const std::string TAG = "ABenAgent";

ABenAgent::ABenAgent(IBenGameplay* gameplay, const std::string& tag) :
	BaseWowBot(tag),
	mGameplay(gameplay),
	mSelf(nullptr),
	mInGame(false),
	mInCombat(false),
	mLastEvalTimestamp(0)
{
}

ABenAgent::~ABenAgent() = default;

bool ABenAgent::attach(std::shared_ptr<WowGame> game) {
	for (auto it = mNestedAgents.begin(); it != mNestedAgents.end(); ++it) {
		(*it)->attach(game);
	}
	return BaseWowBot::attach(game);
}

void ABenAgent::onResume()
{

}

void ABenAgent::onPause()
{
	notifyInCombat(false);
	notifyGamePlay(false);
	mSelf = nullptr;
}

bool ABenAgent::onEvaluate() {
	FileLogger dbg(mDbg, "onEvaluate");

	if (!snapGameFrame())
	{
		dbg << dbg.e() << "ABenAgent:L" << __LINE__ << ": snapGameFrame failed" << dbg.endl();
		return false;
	}

	// trigger onGamePlayStart / onGamePlayStop events
	const auto inGameBefore(nullptr != mSelf);
	mSelf = mGame->getObjectManager().getActivePlayer();
	const auto inGameToggled(inGameBefore != (nullptr != mSelf));

	if (inGameToggled)
	{
		notifyGamePlay(nullptr != mSelf);
	}

	// trigger onGamePlayStart / onGamePlayStop events
	if (nullptr != mSelf)
	{

		const auto backSnapshot = mGameplay->previous();

		if (nullptr != backSnapshot && backSnapshot->getTimestamp() > mLastEvalTimestamp)
		{
			//dbg << dbg.w() << "evaluate" << backSnapshot->getTimestamp() << dbg.endl();
			updateFromSnapshot(backSnapshot);
			mLastEvalTimestamp = backSnapshot->getTimestamp();
		}
		else
		{
			//dbg << dbg.w() << "mBackSnapshot is null" << dbg.endl();
		}

		if (inGameToggled)
		{
			onGamePlayStart();
		}
		notifyInCombat(mSelf->isInCombat());
	}
	else
	{
		if (inGameToggled)
		{
			onGamePlayStart();
		}
	}

	return onEvaluatedIdle();
}

void ABenAgent::onGamePlayStart() {

}

void ABenAgent::onGamePlayStop() {

}

void ABenAgent::onCombatStart() {

}

void ABenAgent::onCombatEnd() {

}

void ABenAgent::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
	mAggroList.push_back(object);
}

void ABenAgent::onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object)
{
	mAggroList.remove(object);
}

bool ABenAgent::isInCombat() const
{
	return mInCombat;
}

bool ABenAgent::runAway()
{
	return false;
}

bool ABenAgent::updateFromSnapshot(const std::shared_ptr<const IBenGameSnapshot>& snapshot)
{

	FileLogger dbg(mDbg, "updateFromSnapshot");
	const auto& objMgr(mGame->getObjectManager());
	auto prevEnemies = snapshot->getHostileList();
	auto enemies = objMgr.allOfType<WowUnitObject>();

	//enemies.remove_if([&](const std::shared_ptr<const WowUnitObject>& v) { return !mSelf->canAttack(*mGame, *v); });

	{
		IBenGameSnapshot::UnitList oldAggroList;
		auto selfGuid(mSelf->getGuid());
		auto predicate = [&selfGuid](const std::shared_ptr<const WowUnitSnapshot>& v)->bool {
			return v->getTargetGuid() == selfGuid;
		};

		std::copy_if(prevEnemies.begin(), prevEnemies.end(), std::back_inserter(oldAggroList), predicate);

		// trigger onUnitAggroLost
		dbg << dbg.w() << "previous aggro list of " << oldAggroList.size() << dbg.endl();
		for (auto it = oldAggroList.begin(); it != oldAggroList.end(); ++it)
		{
			const auto liveAggroUnit = objMgr.getObjectByGuid<WowUnitObject>((*it)->getGuid());

			if (nullptr == liveAggroUnit || liveAggroUnit->vanished() || liveAggroUnit->getTargetGuid() != mSelf->getGuid())
			{
				dbg << dbg.w() << " onUnitAggroLost ts " << snapshot->getTimestamp() << dbg.endl();
				onUnitAggroLost(liveAggroUnit);
			}
		}

		// trigger onUnitAggro
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			const auto& enemy(**it);

			if (enemy.getTargetGuid() == mSelf->getGuid())
			{
				auto oldVersion(snapshot->getUnitByGuid(enemy.getGuid()));

				if (nullptr == oldVersion || oldVersion->getTargetGuid() != mSelf->getGuid())
				{
					dbg << dbg.w() << " onUnitAggro ts " << snapshot->getTimestamp() << dbg.endl();
					onUnitAggro(*it);
				}
				//else
				//	dbg << dbg.w() << " target previously already in aggro? " << oldVersion << dbg.endl();

			}
		}
	}
	return true;
}

bool ABenAgent::handleWowMessage(ServerWowMessage & cl) {
	bool handled = false;
	mDbg << mDbg.v() << "handleServerMessage " << (int)cl.type << mDbg.endl();

	switch (cl.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
	default:
		for (auto it = mNestedAgents.begin(); !handled && it != mNestedAgents.end(); ++it)
			handled = (*it)->handleWowMessage(cl);
		break;
	}

	return handled;
}

void ABenAgent::onGameLoadingStart()
{
	// todo
}

void ABenAgent::onGameLoadingEnd()
{
	// todo
}

bool ABenAgent::snapGameFrame()
{
	if (nullptr != mGameplay && !mGameplay->snap(*mGame))
	{
		return false;
	}
	return true;
}

void ABenAgent::notifyGamePlay(const bool inGameNow)
{
	FileLogger dbg(mDbg, "notifyGamePlay");
	
	mInGame = inGameNow;
	if (mInGame)
	{
		dbg << FileLogger::debug << "evaluated GamePlayStarted" << FileLogger::normal << std::endl;
		onGamePlayStart();
	}
	else
	{
		dbg << FileLogger::debug << "evaluated onGamePlayStop" << FileLogger::normal << std::endl;
		onGamePlayStop();
	}
}

bool ABenAgent::notifyInCombat(const bool inCombatNow)
{
	FileLogger dbg(mDbg, "notifyGamePlay");

	if (inCombatNow != mInCombat)
	{
		if (inCombatNow)
		{
			dbg << FileLogger::debug << "evaluated onCombatStart" << FileLogger::normal << std::endl;
			onCombatStart();
		}
		else
		{
			dbg << FileLogger::debug << "evaluated onCombatEnd" << FileLogger::normal << std::endl;
			onCombatEnd();
		}

		mInCombat = inCombatNow;
		return true;
	}
	return false;
}
