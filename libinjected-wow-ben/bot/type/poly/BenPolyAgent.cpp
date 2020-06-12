#include "BenPolyAgent.h"

#include "../base/BenLoginAgent.h"

BenPolyAgent::BenPolyAgent(BenLoginAgent* login, ABenAgent* characterSelector, ABenAgent* gameAgent) :
	ABenAgent(nullptr, "BenPolyAgent"),
	mLogin(login),
	mCharacterSelector(characterSelector),
	mGameAgent(gameAgent)
{
	if (nullptr != login) mNestedAgents.push_back(login);
	if (nullptr != characterSelector) mNestedAgents.push_back(characterSelector);
	if (nullptr != gameAgent) mNestedAgents.push_back(gameAgent);
}

BenPolyAgent::~BenPolyAgent() = default;

bool BenPolyAgent::onEvaluatedIdle()
{
	FileLogger dbg(mLog, "onEvaluate");
	IWowBot* bot = nullptr;

	if (nullptr != mGameAgent) 
	{
		if (!mGame->isLoading()) 
		{
			if (!mGame->isLoggedIn()) 
			{
				dbg << FileLogger::debug << "game is not logged in" << FileLogger::normal << std::endl;
				bot = mLogin.get();
			}
			else if (!mGame->isInGameOrLoading())
			{
				dbg << FileLogger::debug << "no character selection" << FileLogger::normal << std::endl;
				bot = mCharacterSelector.get();
			}
			else 
			{
				dbg << FileLogger::debug << "tasking" << FileLogger::normal << std::endl;
				bot = mGameAgent.get();
			}
		}
		else 
		{
			dbg << FileLogger::debug << "game is loading" << FileLogger::normal << std::endl;
		}
	}

	if (nullptr != bot) 
	{
		dbg << FileLogger::debug << "evaluated to run " << bot->getTag() << FileLogger::normal << std::endl;
		return bot->onEvaluate();
	}

	dbg << FileLogger::warn << "no idea what to do" << FileLogger::normal << std::endl;
	return false;
}

void BenPolyAgent::assignTask(ABenAgent* task)
{
	mGameAgent.reset(task);
}
