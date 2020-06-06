#include <vector>

#include "ABenAgent.h"

#include "game/WowGame.h"
#include "game/object/WowGuid128.h"
#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"

const std::string TAG = "ABenAgent";

ABenAgent::ABenAgent(IBenGameplay* gameplay, const std::string& tag) :
	BaseWowBot(tag),
	mGameplay(gameplay),
	mWaypoints(nullptr),
	mSelf(nullptr),
	mInGame(false),
	mInCombat(false)
{
}

ABenAgent::~ABenAgent() {
}

bool ABenAgent::attach(std::shared_ptr<WowGame> game) {
	return BaseWowBot::attach(game);
}

void ABenAgent::onResume()
{
	
}

void ABenAgent::onPause()
{
	notifyInCombat(false);
	notifyGamePlay(false);
}

bool ABenAgent::onEvaluate() {
	FileLogger dbg(mDbg, "onEvaluate");

	if (!snapGameFrame())
	{
		return false;
	}

	mSelf = mGame->getObjectManager().getActivePlayer();

	// trigger onGamePlayStart / onGamePlayStop events
	notifyGamePlay(nullptr != mSelf);

	// trigger onGamePlayStart / onGamePlayStop events
	if (mInGame)
	{
		if (nullptr != mSelf)
		{
			notifyInCombat(mSelf->isInCombat());
		}
		else
		{
			// fixme how to handle this?
			dbg << FileLogger::err << "GamePlayStarted, yet no self player was found" << FileLogger::normal << std::endl;
			notifyInCombat(false);
		}
	}

	//dbg << FileLogger::debug << "evaluated game paused " << mInGame << " combat " << mInCombat <<  " self " << mSelf << FileLogger::normal << std::endl;
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

bool ABenAgent::isInCombat() const
{
	return mInCombat;
}

bool ABenAgent::runAway()
{
	return false;
}

bool ABenAgent::handleWowMessage(ServerWowMessage& cl) {
	bool handled = false;
	mDbg << FileLogger::verbose << "handleServerMessage " << (int)cl.type << FileLogger::normal << std::endl;

	switch (cl.type) {
	case MessageType::POST_DLL_DATA_3DPATH:
		mWaypoints = std::make_unique<LinearPathFinder>(*cl.waypoints);
		mDbg << FileLogger::verbose << "Loaded pathfinder with " << cl.waypoints->size() << "waypoints! thanks! " << (int)cl.type << FileLogger::normal << std::endl;
		handled = true;
	default:
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

bool ABenAgent::notifyGamePlay(bool inGameNow)
{
	FileLogger dbg(mDbg, "notifyGamePlay");

	if (inGameNow != mInGame)
	{
		if (inGameNow)
		{
			dbg << FileLogger::debug << "evaluated GamePlayStarted" << FileLogger::normal << std::endl;
			onGamePlayStart();
		}
		else
		{
			dbg << FileLogger::debug << "evaluated onGamePlayStop" << FileLogger::normal << std::endl;
			onGamePlayStop();
		}

		mInGame = inGameNow;
		return true;
	}

	return false;
}

bool ABenAgent::notifyInCombat(bool inCombatNow)
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
