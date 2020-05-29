#include "pch.h"

#include <thread>
#include <sstream>

#include "InjectedClient.h"
#include "Client.h"
#include "../NetworkParsing.h"

/* Observers */
// Observe wow player positions

/* Userspace */

// business
#include "../injectable/wow/observer/ActivePlayerPositionObserver.h"
#include "../injected/plugin/DllFolderPlugin.h"
#include "../injectable/wow/WowPlugin.h"


/* Features */

#include "../injectable/wow/game/WowVector3f.h"

#include "../injectable/wow/ServerWowMessage.h"

InjectedClient::InjectedClient() :
	mDebugger("InjectedClient"),
	mBootTime(GetTickCount64()),
	mLastPulse(0),
	mClient(std::make_unique<Client>())
{
	auto pid = (long)GetCurrentProcessId();

	if (mClient->joinServer()) {
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(pid));
	}

	auto pluginsPath = L"D:\\myplugins";

	auto plugins = std::make_shared<DllFolderPlugin>();
	if (plugins->loadFolder(pluginsPath)) {
		mPlugins.push_back(plugins);
	}
	else {
		// Failed to read from plugins folder, fallback to default "WowPlugin"
		mPlugins.push_back(std::make_shared<WowPlugin>(0));
	}
}

InjectedClient::~InjectedClient() {
	mClient->disconnect();
	mDebugger << FileLogger::warn << "~InjectedClient" << FileLogger::normal << std::endl;
	mDebugger.flush();
}

uint64_t InjectedClient::getBootTime() const { return mBootTime; }
uint64_t InjectedClient::getLastPulse() const { return mLastPulse; }

const void* InjectedClient::id() const {
	return this;
}

bool InjectedClient::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool InjectedClient::run() {
	if (throttle()) return true;
	if (!mClient->isConnected()) return false;

	auto wowPlugin = dynamic_cast<WowPlugin*>(mPlugins.begin()->get());
	if (wowPlugin != nullptr) {
		// dispatch client
		if (!wowPlugin->handleClient(*mClient))
			return false;
	}

	for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
		(*it)->onD3dRender();
	}

	mLastPulse = GetTickCount64();
	mDebugger.flush();
	return true;
}
