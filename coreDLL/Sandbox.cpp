#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "d3d/d3d.h"

bool stackServerMessages(ServerSDK& serverSDK, MessageManager* messageManager) {
	std::list<std::string> messages = serverSDK.getMessageAvailable();

	for (std::list<std::string>::const_iterator it = messages.begin(); it != messages.end(); it++)
	{
		switch (messageManager->getMessageType((*it))) {
		case MessageType::START_SUBSCRIBE: {
			std::list<std::string> toSubscribe = serverSDK.getMessageManager().getSubcribeObject(*it);
			bool subToPosition = std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end();

			if (subToPosition) {
				// fake a position response, just for temporary debug
				serverSDK.sendMessage(messageManager->builResponseInfo("position", "X,Y,Z"));
			}

			break;
		}
		case MessageType::STOP_SUBSCRIBE: {
			std::list<std::string> toSubscribe = serverSDK.getMessageManager().getSubcribeObject(*it);
			/*
			To be removed, just for testing purpose...
			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
			if (found)
				serverSDK.sendMessage(serverSDK.getMessageManager().builResponseInfo("position", "NONONO"));
			*/
			break;
		}
		case MessageType::DEINJECT:
			return false;
		default:
			// Error, unknown MessageType? what are you mumbling chief?!
			// should I return false and shoot myself?
			// TODO: return false;
			break;
		}
	}
	return true;
}

Sandbox::Sandbox()
	:
	mModuleBaseAddr(GetModuleHandleA(0)),
	mDebugger("D:\\nvtest.log"),
	mPid(GetCurrentProcessId()),
	bootTime(0), lastPulse(0),
	mWindowController(FindMainWindow(mPid)),
	mGame((const uint8_t*)mModuleBaseAddr),
	mBot(mWindowController, mGame)
{

}

Sandbox::~Sandbox() {
	// disconnect from server in case we are still connected
	mDebugger.log("~Sandbox");
}

const WindowController& Sandbox::getWindowController() const {
	return mWindowController;
}

WindowController& Sandbox::getWindowController() {
	return mWindowController;
}

bool Sandbox::isOverHeating() const {
	return lastPulse + 120 > GetTickCount64();
}

void Sandbox::run() {
	if (isOverHeating()) return;

	bool abort = false;
	std::stringstream ss;

	mGame.update();
	ss << mGame << std::endl;

	mBot.run(mDebugger);

	mDebugger.log(ss.str().c_str());
	mDebugger.flush();
	lastPulse = GetTickCount64();
}
