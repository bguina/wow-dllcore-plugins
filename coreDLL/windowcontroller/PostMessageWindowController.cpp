#include "pch.h"

#include "../framework.h" // required for "PostMessage"

#include <cmath>
#include <iostream>
#include <map>

#include "../logger/FileLogger.h"
#include "PostMessageWindowController.h"

// Observed SendMessage "legit" flags found with Spy++ app
const int SENDMESSAGE_KEYUP_FLAGS = 0xC0110001;
const int SENDMESSAGE_KEYDOWN_FLAGS = 0x00110001;


PostMessageWindowController::PostMessageWindowController(HWND window) :
	mWindow(window),
	mDbg("PostMessageWindowController")
{

}

PostMessageWindowController::~PostMessageWindowController()
{

}

bool PostMessageWindowController::isKeyPressed(WinVirtualKey key) const {
	auto it = mKeyPressedStatus.find(key);
	bool isPressed = it != mKeyPressedStatus.end() && 0 != it->second;

	return isPressed;
}

// Attempt to push down or unpush a key.
// Return true if key is in expected final state.
bool PostMessageWindowController::pressKey(WinVirtualKey key, bool keyDown) {
	bool keyAsRequested(keyDown == isKeyPressed(key));

	if (postKeyEventMessage(key, keyDown)) {
		mKeyPressedStatus.insert(std::pair<WinVirtualKey, uint32_t>(key, keyDown));
		keyAsRequested = true;
	}

	if (false) {
		mDbg << FileLogger::info << "pressKey " << (int)key << " success? " << keyAsRequested << FileLogger::normal << std::endl;
		mDbg.flush();
	}

	return keyAsRequested;
}

void PostMessageWindowController::releaseAllKeys() {

	pressKey(WinVirtualKey::WVK_A, false);
	pressKey(WinVirtualKey::WVK_D, false);
	pressKey(WinVirtualKey::WVK_W, false);

	// fixme key map states
	for (auto it = mKeyPressedStatus.begin(); it != mKeyPressedStatus.end(); it++)
	{
		if (it->second != 0 && postKeyEventMessage(it->first, false)) {
			it->second = 0;
		}
	}
}

bool PostMessageWindowController::postKeyEventMessage(WinVirtualKey key, bool keyDown) {
	int action = WM_KEYUP;
	int flags = SENDMESSAGE_KEYUP_FLAGS;

	if (keyDown) {
		action = WM_KEYDOWN;
		flags = SENDMESSAGE_KEYDOWN_FLAGS;
	}

	return PostMessage(mWindow, action, (uint8_t)key, flags);
}
