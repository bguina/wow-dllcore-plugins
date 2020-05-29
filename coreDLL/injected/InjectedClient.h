#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "../logger/FileLogger.h"
#include "plugin/IPlugin.h"

class MessageManager;
class ClientMessage;
class Client;

class InjectedClient
{
public:
	InjectedClient();
	InjectedClient(InjectedClient const&) = delete;
	void operator=(InjectedClient const&) = delete;
	~InjectedClient();

	bool throttle() const;

	const void* id() const;
	uint64_t getBootTime() const;
	uint64_t getLastPulse() const;

	bool run();

protected:
	uint64_t mBootTime;
	uint64_t mLastPulse;
	FileLogger mDebugger;
	std::list<std::shared_ptr<IPlugin>> mPlugins;

	// TODO move Client to WowPlugin
	std::shared_ptr<Client> mClient;
};

inline std::ostream& operator<<(std::ostream& out,const class InjectedClient& obj) {
	out << "[InjectedClient:" << obj.id() << "," << obj.getLastPulse() << "]";
	// TODO print navigator state
	return out;
}
