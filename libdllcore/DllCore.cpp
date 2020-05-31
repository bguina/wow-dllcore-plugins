#include "pch.h"

#include "DllCore.h"
#include "NetworkParsing.h"
#include "plugin/DllFolderPlugin.h"

DllCore::DllCore() :
	mDbg("DllCore"),
	mBootTime(GetTickCount64()),
	mLastPulse(0)
{
	mDbg << FileLogger::debug << "DllCore()" << FileLogger::normal << std::endl;
}

DllCore::DllCore(IPlugin* plugin) : DllCore() {
	mPlugins.push_back(std::unique_ptr<IPlugin>(plugin));
}

DllCore::~DllCore() {
	mDbg << FileLogger::debug << "~DllCore()" << FileLogger::normal << std::endl;
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

		dbg << FileLogger::debug <<  "call" << FileLogger::normal << std::endl;
		if (!mPlugins.empty()) {
			for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
				dbg << FileLogger::debug << (*it)->getTag() << "->onFrameRender()" << FileLogger::normal << std::endl;

				if (!(*it)->onD3dRender()) {
					mPlugins.remove(*it);
					dbg << FileLogger::warn << " ejection" << FileLogger::normal << std::endl;
					return false; // fixme
				}
			}
		}
		else {
			dbg << FileLogger::err << "no plugin loaded" << FileLogger::normal << std::endl;
			return false;
		}
		mLastPulse = GetTickCount64();
	}
	return true;
}
