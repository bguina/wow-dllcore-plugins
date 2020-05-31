#pragma once

#include <list>
#include <memory>

#include "plugin/IPlugin.h"
#include "FileLogger.h"

class MessageManager;
class ClientMessage;
class Client;

class DllCore {
public:
	DllCore();
	DllCore(IPlugin* plugin);
	DllCore(DllCore const&) = delete;
	void operator=(DllCore const&) = delete;
	~DllCore();

	bool throttle() const;

	const void* id() const;
	uint64_t getBootTime() const;
	uint64_t getLastPulse() const;

	bool onFrameRender();

protected:
	FileLogger mDbg;
	uint64_t mBootTime;
	uint64_t mLastPulse;
	std::list<std::unique_ptr<IPlugin>> mPlugins;
};
