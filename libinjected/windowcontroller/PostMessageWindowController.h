#pragma once
#include <Windows.h>

#include <iostream>
#include <map>

#include "IWindowController.h"

#include "WinVirtualKey.h"

class PostMessageWindowController : public IWindowController
{
public:
	PostMessageWindowController(HWND window);
	virtual ~PostMessageWindowController();

	virtual bool isKeyPressed(WinVirtualKey key) const override;

	// Attempt to push down or unpush a key.
	// Return true if key is in expected final state.
	virtual bool pressKey(WinVirtualKey key, bool keyDown) override;

	virtual void releaseAllKeys() override;

protected:
	FileLogger mDbg;
	HWND mWindow;
	std::map<WinVirtualKey, int> mKeyPressedStatus;

private:
	bool postKeyEventMessage(WinVirtualKey key, bool keyDown);
};
