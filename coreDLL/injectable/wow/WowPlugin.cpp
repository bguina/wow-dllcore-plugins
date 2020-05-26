#include "pch.h"

#include "../../framework.h"

#include "IWowBot.h"
#include "WowPlugin.h"

#include "../../userspace/ben/BenFightBot.h"
#include "../../userspace/ben/BenTravelBot.h"
#include "../../userspace/max/MaxBot.h"

std::unique_ptr<IWowBot> _makeBestAmongAll(WowGame& game) {
	int botSelection = 0;

	switch (botSelection) {
	case 0:
		return std::make_unique<WowMaxBot>(game);
	case 99:
		return std::make_unique<BenTravelBot>(game);
	default:
		return std::make_unique<BenFightBot>(game);
	}
}

WowPlugin::WowPlugin(long pid, HMODULE moduleBaseAddress) :
	mBotPause(true),
	mGame(pid, (const uint8_t*)moduleBaseAddress),
	mDbg("WowPlugin"),
	mBot(_makeBestAmongAll(mGame))
{
	mDbg << FileLogger::verbose << "chose the best fighter: " << mBot->getTag() << FileLogger::normal << std::endl;
}

WowPlugin::~WowPlugin()
{
}

void WowPlugin::onD3dRender() {
	if (!mBotPause) {
		mGame.update();
		mBot->onEvaluate();
	}
	mDbg.flush();
}

bool WowPlugin::handleServerMessage(ClientMessage& serverMessage) {
	mDbg << FileLogger::verbose << "serverMessage: " << (int)serverMessage.type << FileLogger::normal << std::endl;
	switch (serverMessage.type) {
	case MessageType::RESUME:
		mBotPause = false;
		mBot->onResume();
		return true;
	case MessageType::PAUSE:
		mBotPause = true;
		mBot->onPause();
		return true;
	default: return mBot->handleServerMessage(serverMessage);
	}
}
