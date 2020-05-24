#pragma once

#include <iostream>

#include "../debugger/FileDebugger.h"
#include "process/wow/WowGame.h"
#include "plugins/IPlugin.h"

class MessageManager;
class PluginServerMessage;
class Client;

class InjectedClient
{
public:
	InjectedClient();
	~InjectedClient();

	bool throttle() const;

	const void* id() const;
	uint64_t getBootTime() const;
	uint64_t getLastPulse() const;
	const WowGame& getGame() const;

	bool run();

private:
	PluginServerMessage _buildMessage(const std::string& messageId);
	bool _dispatchMessages();

	uint64_t mBootTime;
	uint64_t mLastPulse;
	std::unique_ptr<Client> mClient;
	FileDebugger mDebugger;
	WowGame mGame;
	std::unique_ptr<IPlugin> mPlugin;

public:
	InjectedClient(InjectedClient const&) = delete;
	void operator=(InjectedClient const&) = delete;
};

inline std::ostream& operator<<(std::ostream& out,const class InjectedClient& obj) {
	out << "[InjectedClient:" << obj.id() << "," << obj.getLastPulse() << "]";
	// TODO print navigator state
	return out;
}
