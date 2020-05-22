#pragma once

#include "WinVirtualKey.h"

class IWindowController
{
public:
	virtual bool isKeyPressed(WinVirtualKey key) const = 0;

	// Attempt to push down or unpush a key.
	// Return true if key is in expected final state.
	virtual bool pressKey(WinVirtualKey key, bool keyDown) = 0;

	virtual void releaseAllKeys() = 0;
};
