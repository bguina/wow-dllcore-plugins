#pragma once

#include "framework.h"

#include <cmath>
#include <iostream>

#include "Debugger.h"
#include "WinVirtualKey.h"

class WindowController
{
public:
	WindowController(
		HWND window
	) : mWindow(window)
	{}

	bool isKeyPressed(WinVirtualKey key) const {
		bool keyPushedLie = false;
		// lie, we don't know
		return keyPushedLie;
	}

	// Attempt to push down or unpush a key.
	// Return true if key is in expected final state.
	bool press(WinVirtualKey key, bool keyDown) {
		bool keyAlreadyPushed = false;

		return postKeyEvent(key, keyDown);
	}

protected:
	HWND mWindow;

private:
	// Return true if key event is sent.
	bool postKeyEvent(WinVirtualKey key, bool keyDown) {
		int action = WM_KEYUP;
		int flags = SENDMESSAGE_KEYUP_FLAGS;	

		if (keyDown) {
			action = WM_KEYDOWN;
			flags = SENDMESSAGE_KEYDOWN_FLAGS;
		}

		return PostMessage(mWindow, action, (uint8_t)key, flags);
	}

	// Observed SendMessage "legit" flags found with Spy++ app
	const int SENDMESSAGE_KEYUP_FLAGS = 0x00110001;
	const int SENDMESSAGE_KEYDOWN_FLAGS = 0x00110001;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WindowController& obj
	)
{
	out << "[WindowController]";
	// TODO print controller key states?
	return out;
}
