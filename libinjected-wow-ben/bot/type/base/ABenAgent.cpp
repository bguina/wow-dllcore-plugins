#include "ABenAgent.h"

#include "game/WowGame.h"
#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"
#include "evaluator/ABenWowGameEvaluator.h"
#include "evaluator/IBenEvaluator.h"

const std::string TAG = "ABenAgent";

ABenAgent::ABenAgent(ABenWowGameEvaluator* gameplay, const std::string& tag) :
	BaseWowBot(tag),
	mGameplay(gameplay),
	mSelf(nullptr)
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
	onGamePlayStart();
}

void ABenAgent::onPause()
{
	onGamePlayStop();
}

bool ABenAgent::onEvaluate() {
	FileLogger dbg(mDbg, "onEvaluate");

	mSelf = mGame->getObjectManager().getActivePlayer();

	if (nullptr == mGameplay || !mGameplay->read(*mGame) || !mGameplay->evaluate())
	{
		dbg << dbg.e() << "ABenAgent:L" << __LINE__ << ": mGameplay read / evaluate failed" << dbg.endl();
		return false;
	}
	
	const auto& result(mGameplay->getResult());
	
	if (result.gameEntered) onGamePlayStart();
	else if (result.gameLeft) onGamePlayStop();

	if (nullptr != mSelf)
	{
		if (result.combatEntered) onCombatStart();
		else if (result.combatLeft) onCombatEnd();
		
		for (auto it = result.appearList.begin(); it != result.appearList.end(); ++it)
			onUnitAppear(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		
		for (auto it = result.vanishList.begin(); it != result.vanishList.end(); ++it)
			onUnitVanish(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));

		for (auto it = result.deathList.begin(); it != result.deathList.end(); ++it)
			onUnitDeath(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
		
		for (auto it = result.aggroList.begin(); it != result.aggroList.end(); ++it)
			onUnitAggro(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));

		for (auto it = result.aggroLostList.begin(); it != result.aggroLostList.end(); ++it)
			onUnitAggroLost(mGame->getObjectManager().getObjectByGuid<WowUnitObject>(*it));
	}

	//dbg << dbg.d() << "ABenAgent:L" << __LINE__ << ": onEvaluatedIdle" << dbg.endl();
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

void ABenAgent::onUnitAppear(const std::shared_ptr<const WowUnitObject>& object)
{
	
}

void ABenAgent::onUnitVanish(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenAgent::onUnitDeath(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenAgent::onUnitAggro(const std::shared_ptr<const WowUnitObject>& object)
{
}

void ABenAgent::onUnitAggroLost(const std::shared_ptr<const WowUnitObject>& object)
{
}

bool ABenAgent::runAway()
{
	return false;
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

