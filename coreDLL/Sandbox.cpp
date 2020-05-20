#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "observers/ActivePlayerPositionObserver.h"
#include "d3d/d3d.h"

bool readMessageAvailable(ServerSDK& server, WowGame& game) {
	std::list<std::string> messages = server.getMessageAvailable();
	for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		switch (server.getMessageManager().getMessageType((*it)))
		{
		case MessageType::START_SUBSCRIBE: {
			std::list<std::string> toSubscribe = server.getMessageManager().getSubcribeObject(*it);

			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

			if (found) {
				game.addObserver("position", new ActivePlayerPositionObserver(server, 1));
			}

			break;
		}
		case MessageType::STOP_SUBSCRIBE: {
			std::list<std::string> toSubscribe = server.getMessageManager().getSubcribeObject(*it);
			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
			if (found) {
				game.removeObserver("position");
			}

			break;
		}
		case MessageType::WAYPOINTS: {
			std::list<std::string> listWaypoint = server.getMessageManager().getWaypoinsObject(*it);

			// load Navigator with a waypoints profile
			break;
		}
		case MessageType::DEINJECT: {
			return false;
			break;
		}
		default:
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

bool Sandbox::run(ServerSDK& server) {
	if (isOverHeating()) return true;

	bool abort = false;
	std::stringstream ss;

	mGame.update();
	ss << mGame << std::endl;

	mBot.run(mDebugger);

	if (!readMessageAvailable(server, mGame)) return false;

	mDebugger.log(ss.str().c_str());
	mDebugger.flush();
	lastPulse = GetTickCount64();
	return true;
}
