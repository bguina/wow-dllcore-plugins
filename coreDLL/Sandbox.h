#pragma once

class Sandbox
{
public:
	static class Sandbox& getInstance()
	{
		static Sandbox instance;
		return instance;
	}

	Sandbox(Sandbox const&) = delete;
	void operator=(Sandbox const&) = delete;

	void run() {

		std::stringstream ss;

		ss << mGame << std::endl;
		ss << mBot << std::endl;
		mBot.run();

		mLogger.log(ss.str());
	}

private:
	Sandbox()
		: mModuleBaseAddr((uint8_t*)GetModuleHandleA(0)),
		mGame(mModuleBaseAddr),
		mBot(WowNavigator(FindMainWindow(GetCurrentProcessId()), mGame)),
		mLogger("D:\\nvtest.log")
	{}

	uint8_t* mModuleBaseAddr;
	WowGame mGame;
	WowNavigator mBot;
	Logger mLogger;
};
