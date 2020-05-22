#pragma once

#include <iostream>

#include "injected/wow/WowGame.h"
#include "injected/wow/bot/WowBot.h"
#include "ServerSDK.h"
#include "MessageManager.h"
#include "debugger/FileDebugger.h"

class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	bool throttle() const;

	const void* id() const {
		return this;
	}

	ULONG64 getBootTime() const;
	ULONG64 getLastPulse() const;
	const WowGame& getGame() const;

	bool run(ServerSDK& server);

private:
	bool stackServerMessages(ServerSDK& server);

	ULONG64 mBootTime;
	ULONG64 mLastPulse;
	FileDebugger mDebugger;
	WowGame mGame;
	WowBot mBot;

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
