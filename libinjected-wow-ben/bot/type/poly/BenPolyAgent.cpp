#include "BenPolyAgent.h"

#include "../base/BenLoginAgent.h"

BenPolyAgent::BenPolyAgent(BenLoginAgent* login, ABenAgent* characterSelector, ABenAgent* task) :
	ABenAgent(nullptr, "BenPolyAgent"),
	mLogin(login),
	mCharacterSelector(characterSelector),
	mTask(task)
{

}

BenPolyAgent::~BenPolyAgent() = default;

bool BenPolyAgent::onEvaluatedIdle() {
	FileLogger dbg(mDbg, "onEvaluate");
	IWowBot* bot = nullptr;

	if (nullptr != mTask) {
		if (!mGame->isLoading()) {
			if (!mGame->isLoggedIn()) {
				dbg << FileLogger::debug << "game is not logged in" << FileLogger::normal << std::endl;
				bot = mLogin.get();
			}
			else if (!mGame->isInGameOrLoading()) {
				dbg << FileLogger::debug << "no character selection" << FileLogger::normal << std::endl;
				bot = mCharacterSelector.get();
			}
			else {
				dbg << FileLogger::debug << "tasking" << FileLogger::normal << std::endl;
				bot = mTask.get();
			}
		}
		else {
			dbg << FileLogger::debug << "game is loading" << FileLogger::normal << std::endl;
		}
	}

	if (nullptr != bot) {
		dbg << FileLogger::debug << "evaluated to run " << bot->getTag() << FileLogger::normal << std::endl;

		return bot->onEvaluate();
	}
	else {
		dbg << FileLogger::warn << "no idea what to do" << FileLogger::normal << std::endl;
		// todo disconnect
	}
	return false;
}

void BenPolyAgent::assignTask(IWowBot* task) {
	mTask.reset(task);
}
