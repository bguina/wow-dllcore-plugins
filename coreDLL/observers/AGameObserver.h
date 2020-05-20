#pragma once

#include "ServerSDK.h"

#include "IGameObserver.h"

template<class T>
class AGameObserver : public IGameObserver<T> {
protected:
	AGameObserver(unsigned long periodMs)
		:
		mLastCapture(0),
		mPeriodMs(periodMs)
	{}

	virtual void capture(const T& game) final {
		auto now = GetTickCount64();

		if (mLastCapture + mPeriodMs < GetTickCount64()) {
			makeCapture(game);
			mLastCapture = now;
		}
	}

	virtual void makeCapture(const T& game) = 0;

protected:
	unsigned long long mLastCapture;
	unsigned long mPeriodMs;
};
