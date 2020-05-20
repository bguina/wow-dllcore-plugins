#pragma once

#include <iostream>

#include "WowGame.h"
#include "WowNavigator.h"
#include "WindowController.h"
#include "ServerSDK.h"
#include "MessageManager.h"
#include "Debugger.h"

class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	const WindowController& getWindowController() const;
	WindowController& getWindowController();

	bool isOverHeating() const;

	const void* id() const {
		return this;
	}

	ULONG64 getBootTime() const { return bootTime; }
	ULONG64 getLastPulse() const { return lastPulse; }

	bool run(ServerSDK& server);

private:
	HMODULE mModuleBaseAddr;
	long mPid;
	ULONG64 bootTime;
	ULONG64 lastPulse;
	Debugger mDebugger;
	WindowController mWindowController;
	WowGame mGame;
	WowNavigator mBot;

public:
	Sandbox(Sandbox const&) = delete;
	void operator=(Sandbox const&) = delete;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class Sandbox& obj
	)
{
	out << "[Sandbox:" << obj.id() << "," << obj.getLastPulse() << "]";
	// TODO print navigator state
	return out;
}
