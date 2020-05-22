#pragma once

#include "framework.h"

#include <iostream>
#include <map>

#include "debugger/FileDebugger.h"
#include "WinVirtualKey.h"

class WindowController
{
public:
	WindowController(HWND window);

	bool isKeyPressed(WinVirtualKey key) const;

	// Attempt to push down or unpush a key.
	// Return true if key is in expected final state.
	bool pressKey(WinVirtualKey key, bool keyDown);

	void releaseAllKeys();

protected:
	HWND mWindow;
	std::map<WinVirtualKey, int> mKeyPressedStatus;

private:
	bool postKeyEventMessage(WinVirtualKey key, bool keyDown);
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
