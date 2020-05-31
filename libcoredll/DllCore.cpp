#include "pch.h"

#include "DllCore.h"
#include "NetworkParsing.h"
#include "plugin/DllFolderPlugin.h"

DllCore::DllCore(IPlugin* plugin) :
	mDbg("DllCore"),
	mBootTime(GetTickCount64()),
	mLastPulse(0)
{
	mPlugins.push_back(std::shared_ptr<IPlugin>(plugin));
}

DllCore::~DllCore() {
	mDbg << FileLogger::warn << "~DllCore" << FileLogger::normal << std::endl;
}

uint64_t DllCore::getBootTime() const { return mBootTime; }
uint64_t DllCore::getLastPulse() const { return mLastPulse; }

const void* DllCore::id() const {
	return this;
}

bool DllCore::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool DllCore::onFrameRender() {
	if (!throttle()) {
		FileLogger dbg(mDbg, "onFrameRender");

		if (!mPlugins.empty()) {
			for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
				dbg << FileLogger::debug << (*it)->getTag() << "->onFrameRender()" << FileLogger::normal << std::endl;

				if (!(*it)->onD3dRender()) {
					mPlugins.remove(*it);
					dbg << FileLogger::warn << " ejection" << FileLogger::normal << std::endl;
					return false;
				}
			}
		}
		else
			dbg << FileLogger::err << "no plugin loaded" << FileLogger::normal << std::endl;

		mLastPulse = GetTickCount64();
	}
	return true;
}
