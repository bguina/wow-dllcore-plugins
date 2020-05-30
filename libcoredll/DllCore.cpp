#include "pch.h"

#include <thread>
#include <sstream>

#include "DllCore.h"
#include "NetworkParsing.h"

/* Observers */
// Observe wow player positions

/* Userspace */

// business
#include "injected/plugin/DllFolderPlugin.h"
//#include "observer/ActivePlayerPositionObserver.h"
//#include "WowPlugin.h"


/* Features */

//#include "game/WowVector3f.h"

//#include "ServerWowMessage.h"

DllCore::DllCore(IPlugin* plugin) :
	mDbg("DllCore"),
	mBootTime(GetTickCount64()),
	mLastPulse(0)
{
	mPlugins.push_back(std::shared_ptr<IPlugin>(plugin));
}

DllCore::~DllCore() {
	//mClient->disconnect();
	mDbg << FileLogger::warn << "~DllCore" << FileLogger::normal << std::endl;
	mDbg.flush();
}

uint64_t DllCore::getBootTime() const { return mBootTime; }
uint64_t DllCore::getLastPulse() const { return mLastPulse; }

const void* DllCore::id() const {
	return this;
}

bool DllCore::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool DllCore::run() {
	if (throttle()) return true;

	for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
		if (!(*it)->onD3dRender()) {
			mDbg.flush();
			return false;
		}
	}

	mLastPulse = GetTickCount64();
	mDbg.flush();
	return true;
}
