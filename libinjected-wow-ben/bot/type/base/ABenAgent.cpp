#include "ABenAgent.h"

#include <utility>

#include "game/WowGame.h"
#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"
#include "evaluator/IBenWowGameEvaluator.h"
#include "evaluator/IBenEvaluator.h"

const std::string TAG = "ABenAgent";

ABenAgent::ABenAgent(std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>> gameplay, const std::string& tag) :
	BaseWowBot(tag),
	mGameplay(std::move(gameplay)),
	mSelf(nullptr)
{
}

ABenAgent::~ABenAgent() = default;

void ABenAgent::attach(std::shared_ptr<WowGame> game)
{
	BaseWowBot::attach(game);
	
	for (auto it = mNestedAgents.begin(); it != mNestedAgents.end(); ++it)
		(*it)->attach(game);
}

void ABenAgent::onResume()
{
	BaseWowBot::onResume();
	FileLogger dbg(mLog, "ABenAgent:onResume");

	dbg << dbg.d() << "resume" << dbg.endl();
	for (auto it = mNestedAgents.begin(); it != mNestedAgents.end(); ++it)
		(*it)->onResume();
	
	if (nullptr != mGame->getObjectManager().getActivePlayer())
		onGamePlayStart();
}

void ABenAgent::onPause()
{
	FileLogger dbg(mLog, "ABenAgent:onPause");
	
	dbg << dbg.d() << "pause" << dbg.endl();
	if (nullptr != mSelf)
		onGamePlayStop();

	for (auto it = mNestedAgents.begin(); it != mNestedAgents.end(); ++it)
		(*it)->onPause();
	
	mSelf = nullptr;
	BaseWowBot::onPause();
}

bool ABenAgent::onEvaluate()
{
	FileLogger dbg(mLog, "ABenAgent:onEvaluate");

	if (nullptr == mGame)
	{
		dbg << dbg.e() << "ABenAgent:L" << __LINE__ << ": nullptr == mGame" << dbg.endl();
		return false;
	}
	
	notifyGameplay();

	//dbg << dbg.d() << "ABenAgent:L" << __LINE__ << ": onEvaluatedIdle" << dbg.endl();
	return onEvaluatedIdle();
}

bool ABenAgent::handleWowMessage(ServerWowMessage & cl)
{
	bool handled = false;
	mLog << mLog.v() << "handleServerMessage " << (int)cl.type << mLog.endl();

	switch (cl.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
	default:
		for (auto it = mNestedAgents.begin(); !handled && it != mNestedAgents.end(); ++it)
			handled |= (*it)->handleWowMessage(cl);
		break;
	}

	return handled;
}

bool ABenAgent::faceAngleDegrees(const int angle, const int anglePrecision)
{
	// angle 40
	FileLogger dbg(mLog, "ABenAgent:faceAngleDegrees");
	const auto delta(angle - mSelf->getFacingDegrees());

	// delta  -270
	if (abs(delta) > anglePrecision)
	{
		dbg << dbg.e() << "angle " << angle << " delta " << delta << dbg.endl();
		mGame->getWindowController()->pressKey(WinVirtualKey::WVK_A, delta > anglePrecision);
		mGame->getWindowController()->pressKey(WinVirtualKey::WVK_D, delta < -anglePrecision);
		return false;
	}

	mGame->getWindowController()->releaseAllKeys();
	return true;
}

bool ABenAgent::runAway()
{
	return false;
}

void ABenAgent::notifyGameplay()
{
	FileLogger dbg(mLog, "ABenAgent:notifyGameplay");

	mSelf = mGame->getObjectManager().getActivePlayer();
	if (nullptr == mGameplay)
	{
		dbg << dbg.w() << "ABenAgent:L" << __LINE__ << ": nullptr == mGameplay" << dbg.endl();
		return;
	}
	
	if (!mGameplay->read(*mGame) || !mGameplay->evaluate())
	{
		dbg << dbg.e() << "ABenAgent:L" << __LINE__ << ": mGameplay read / evaluate failed" << dbg.endl();
		return;
	}

	//return true;// second 5-stars michelin crash toggle

	const auto& result(mGameplay->getResult());

	if (result.gameEntered) onGamePlayStart();
	else if (result.gameLeft) onGamePlayStop();

	if (nullptr != mSelf)
	{
		if (result.combatEntered) onCombatStart();
		else if (result.combatLeft) onCombatEnd();

		for (auto it = result.popList.begin(); it != result.popList.end(); ++it)
			onUnitPop(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.depopList.begin(); it != result.depopList.end(); ++it)
			onUnitDepop(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.deathList.begin(); it != result.deathList.end(); ++it)
			onUnitDeath(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.aggroList.begin(); it != result.aggroList.end(); ++it)
			onUnitAggro(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.aggroLostList.begin(); it != result.aggroLostList.end(); ++it)
			onUnitAggroLost(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.tappedList.begin(); it != result.tappedList.end(); ++it)
			onUnitTap(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		for (auto it = result.untappedList.begin(); it != result.untappedList.end(); ++it)
			onUnitUntap(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
	}
}

void ABenAgent::onGameLoadingStart()
{
	// todo
}

void ABenAgent::onGameLoadingEnd()
{
	// todo
}

void ABenAgent::onGamePlayStart()
{
	FileLogger dbg(mLog, "ABenAgent:onGamePlayStart");

	dbg << dbg.d() << "call" << dbg.endl();
	mSelf = mGame->getObjectManager().getActivePlayer();
	
	if (nullptr == mSelf)
	{
		dbg << dbg.e() << "nullptr == getActivePlayer()" << dbg.endl();
	}
	
	if (nullptr != mSelf)
	{
		if (!mSelf->isInCombat()) 
			mSelf->jump();
	}
}

void ABenAgent::onGamePlayStop()
{
	FileLogger dbg(mLog, "ABenAgent:onGamePlayStop");

	dbg << dbg.d() << "call" << dbg.endl();
	mSelf = nullptr;
	
	const auto self(mGame->getObjectManager().getActivePlayer());
	if (nullptr != self)
	{
		if (!self->isInCombat())
			self->sitDown();
	}
}

void ABenAgent::onCombatStart()
{
	FileLogger dbg(mLog, "ABenAgent:onCombatStart");

	dbg << dbg.d() << "call" << dbg.endl();
}

void ABenAgent::onCombatEnd()
{
	FileLogger dbg(mLog, "ABenAgent:onCombatEnd");

	dbg << dbg.d() << "onCombatEnd" << dbg.endl();
}

void ABenAgent::onUnitPop(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitDepop(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitDeath(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitAggro(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitAggroLost(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitTap(const std::shared_ptr<const WowUnitObject> & object)
{
}

void ABenAgent::onUnitUntap(const std::shared_ptr<const WowUnitObject> & object)
{
}

//bool ABenAgent::castSpell(const ISpellDefinition* spell, const uint128_t* target)
//{
//	mGame->getSpellBook().castSpell(*mGame, spell->getSpellId(), target);
//	return true;
//}
