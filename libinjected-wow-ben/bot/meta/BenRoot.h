#pragma once

#include "bot/IWowBot.h"
#include "../base/ABen.h"
#include "../base/BenLogin.h"

enum class State {
	LoggedOut,
	TaskCompletion,
	TaskCompleted
};

enum class Trigger {
	Disconnected,
	Connected,
	TaskCompleted,
};

class BenPoly : public ABen
{
public:
	BenPoly(BenLogin* login, ABen* characterSelector, ABen* task) :
		ABen("BenPoly"),
		mLogin(login),
		mCharacterSelector(characterSelector),
		mTask(task)
	{

	}

	virtual ~BenPoly() {

	}

	virtual bool onEvaluatedIdle() override {
		FileLogger dbg(mDbg, "onEvaluate");
		IWowBot* bot = nullptr;

		if (nullptr != mTask) {
			if (!mGame->isLoading()) {
				if (!mGame->isLoggedIn()) {
					dbg << FileLogger::debug << "game is not logged in" << FileLogger::normal << std::endl;
					bot = mLogin.get();
				}
				else if (!mGame->isInGame()) {
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
			bot->onEvaluate();
			return true; // fixme
		}
		else {
			dbg << FileLogger::warn << "no idea what to do" << FileLogger::normal << std::endl;
			// todo disconnect
		}
		return false;
	}

	void assignTask(IWowBot* task) {
		mTask.reset(task);
	}

protected:
	std::unique_ptr<IWowBot> mLogin;
	std::unique_ptr<IWowBot> mCharacterSelector;
	std::unique_ptr<IWowBot> mTask;
};
