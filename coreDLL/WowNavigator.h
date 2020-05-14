#pragma once

#include <iostream>
#include "WowGame.h"
#include "Windows.h"

class WowNavigator
{
public:
	WowNavigator(
		WowGame game
	) : mGame(game), mForward(false) {}

	void run() {
		WowUnitObject self = mGame.getObjectManager().getSelf();

		moveForward(self.getFacing() > 3.0);
	}

	bool isMovingForward() {
		return mForward;
	}

protected:
	WowGame& mGame;
	bool mForward;

private:
	void moveForward(boolean doMove) {
		HWND windowHandle = mGame.getWindowHandle();

		// Keycodes found at http:// ...
		int forwardKey = 0x57; // 'Z'
		int leftKey = 0x51; // 'Q'
		int rightKey = 0x41; // 'A'

		// Flags found with Spy++
		int keyDownFlags = 0x00110001;
		int keyUpFlags = 0xC0110001;

		if (doMove) {
			PostMessage(windowHandle, WM_KEYDOWN, forwardKey, keyDownFlags);
			mForward = true;
		}
		else {
			PostMessage(windowHandle, WM_KEYUP, forwardKey, keyUpFlags);
			mForward = false;
		}
	}
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowNavigator& obj
	)
{
	// TODO print navigator state
	return out;
}
