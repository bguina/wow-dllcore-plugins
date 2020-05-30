#pragma once

#include "framework.h"
#include <Windows.h>

#include "IServerObserver.h"

#include "FileLogger.h"

template<class T>
class ARecurrentServerObserver : public IServerObserver<T> {
protected:
	ARecurrentServerObserver(const std::string& tag, unsigned long periodMs) :
		mLastCapture(0),
		mPeriodMs(periodMs),
		mDbg(tag)
	{}

public:
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

protected:
	virtual void makeCapture(const T& game) = 0;

	unsigned long long mLastCapture;
	unsigned long mPeriodMs;
	FileLogger mDbg;
};
