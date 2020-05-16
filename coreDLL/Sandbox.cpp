#include "pch.h"

#include <sstream>

#include "Sandbox.h"
#include "d3d/d3d.h"

Sandbox::Sandbox(madeSingleton) : Singleton<Sandbox>(),
	mModuleBaseAddr(GetModuleHandleA(0)),
	mGame((const uint8_t*)mModuleBaseAddr),
	mBot(WowNavigator(mGame))
{}

bool Sandbox::isOverHeating() const {
	static ULONG64 bootTime = 0;
	static ULONG64 lastPulse = 0;

	auto uptime = GetTickCount64();
	if (lastPulse + 120 > uptime)
		return true;

	lastPulse = uptime;
	return false;
}

void Sandbox::run() {

	std::stringstream ss;

	ss << mGame << std::endl;
	ss << mBot << std::endl;
	mBot.run();

	Debugger::getInstance().log(ss.str().c_str());
}
