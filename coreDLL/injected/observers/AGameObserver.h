#pragma once

#include "IGameObserver.h"

#include "../../debugger/FileDebugger.h"

template<class T>
class AGameObserver : public IGameObserver<T> {
protected:
	AGameObserver(const std::string& tag, unsigned long periodMs)
		:
		mLastCapture(0),
		mPeriodMs(periodMs),
		mDbg(tag)
	{}

	virtual void capture(const T& game) final {
		auto now = GetTickCount64();

		if (mLastCapture + mPeriodMs < GetTickCount64()) {
			makeCapture(game);
			mLastCapture = now;
		}
		else {
			mDbg.i("not capturing");
		}

		mDbg.flush();
	}

	virtual void makeCapture(const T& game) = 0;

protected:
	unsigned long long mLastCapture;
	unsigned long mPeriodMs;
	FileDebugger mDbg;
};
