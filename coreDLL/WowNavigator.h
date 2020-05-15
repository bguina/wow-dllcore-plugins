#pragma once

#include <cmath>
#include <iostream>
#include "WowGame.h"
#include "Windows.h"

class WowNavigator
{
public:
	WowNavigator(
		HWND window,
		WowGame& game
	) :
		mWindow(window),
		mGame(game),
		mForward(false)
	{}



	void run( ) {
		if (!mGame.isObjectManagerActive())
			return;

		WowUnitObject self = WowUnitObject(mGame.getObjectManager().getSelf());
		const Vector3f& pos = self.getPosition();

		auto someBoar = mGame.getObjectManager().getSomeBoar();
		if (NULL == someBoar)
			return;

		const uint32_t* initialGuid = WowUnitObject(someBoar).getGuidPointer();

		//interact(initialGuid);


		// face north
		Vector3f north = Vector3f(pos);
		north.x = pos.x + 500.00f;
		face(north);

		//moveForward(self.getFacingRadians() > 3.0);
	}

	void face(const Vector3f& to) {
		auto selfPos = WowObject(mGame.getObjectManager().getSelf()).getPosition();
		auto deltaX = to.x - selfPos.x;
		auto deltaY = to.y - selfPos.y;
		auto rad = atan2(deltaY, deltaX);
	}

	__int64 interact(const uint32_t* targetGuid) {
		__int64(__fastcall * UnitInteract)(const uint32_t*) = (__int64(__fastcall*)(const uint32_t*))(mGame.getBaseAddress() + 0xD65D60);

		// 0xd65d60
		return UnitInteract(targetGuid);
	}

	bool isMovingForward() {
		return mForward;
	}

	const WowGame& getGame() const {
		return mGame;
	}

	HWND getWindowHandle() const {
		return mWindow;
	}

protected:
	HWND mWindow;
	WowGame& mGame;
	bool mForward;

private:
	void moveForward(boolean doMove) {
		// Keycodes found at https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		int forwardKey = 0x57; // 'W'
		int strafeLeft = 0x51; // 'Q'
		int leftKey = 0x41; // 'A'

		// Flags found with Spy++
		int keyDownFlags = 0x00110001;
		int keyUpFlags = 0xC0110001;

		if (doMove) {
			PostMessage(mWindow, WM_KEYDOWN, forwardKey, keyDownFlags);
			mForward = true;
		}
		else {
			PostMessage(mWindow, WM_KEYUP, forwardKey, keyUpFlags);
			mForward = false;
		}
	}
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	out << "[WowNavigator] found window handle " << obj.getWindowHandle();
	// TODO print navigator state
	return out;
}
