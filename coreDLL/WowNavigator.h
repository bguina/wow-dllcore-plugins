#pragma once

#include <iostream>
#include "WowGame.h"
#include "Windows.h"

class WowNavigator
{
public:
	WowNavigator(
		HWND window,
		WowGame game
	) : mWindow(window), mGame(game), mForward(false) {}

	__int64 interact(const uint32_t* targetGuid) {

		__int64(__fastcall * sub_D65D60)(const uint32_t*) = (__int64(__fastcall*)(const uint32_t*))0xD65D60;
		// d65d60
		return sub_D65D60(targetGuid);
	}

	void run() {
		WowUnitObject self = WowUnitObject(mGame.getObjectManager().getSelf());
		const uint32_t* initialGuid = WowUnitObject(mGame.getObjectManager().getSomeBoar()).getGuidPointer();

		uint32_t* copy = new uint32_t[4];
		copy[0] = initialGuid[0];
		copy[1] = initialGuid[1];
		copy[2] = initialGuid[2];
		copy[3] = initialGuid[3];

		interact(copy);
		//moveForward(self.getFacingRadians() > 3.0);
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
