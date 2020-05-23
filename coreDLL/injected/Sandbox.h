#pragma once

#include <iostream>

#include "../debugger/FileDebugger.h"
#include "wow/WowGame.h"
#include "wow/bot/WowBot.h"

class Client;

class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	bool throttle() const;

	const void* id() const;
	uint64_t getBootTime() const;
	uint64_t getLastPulse() const;
	const WowGame& getGame() const;

	bool run();

private:
	bool stackServerMessages();

	uint64_t mBootTime;
	uint64_t mLastPulse;
	std::unique_ptr<Client> mClient;
	FileDebugger mDebugger;
	WowGame mGame;
	std::unique_ptr<WowBot> mBot;

public:
	Sandbox(Sandbox const&) = delete;
	void operator=(Sandbox const&) = delete;
};

inline std::ostream& operator<<(std::ostream& out,const class Sandbox& obj) {
	out << "[Sandbox:" << obj.id() << "," << obj.getLastPulse() << "]";
	// TODO print navigator state
	return out;
}
