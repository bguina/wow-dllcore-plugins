#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "observers/ActivePlayerPositionObserver.h"

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

Sandbox::Sandbox() :
	mDebugger("D:\\nvtest.log"),
	mBootTime(GetTickCount64()), mLastPulse(0),
	mGame((const uint8_t*)GetModuleHandleA(0)),
	mBot(mGame)
{
	// TODO connect to server from here

}

Sandbox::~Sandbox() {
	// TODO disconnect from server in case we are still connected

	mDebugger.log("~Sandbox");
	mDebugger.flush();
}

ULONG64 Sandbox::getBootTime() const { return mBootTime; }
ULONG64 Sandbox::getLastPulse() const { return mLastPulse; }
WowGame Sandbox::getGame() const { return mGame; }

bool Sandbox::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool Sandbox::run(ServerSDK& server) {
	if (throttle()) return true;

	bool abort = !readMessageAvailable(server, mGame);

	mGame.update();
	mBot.run(mDebugger);

	if (true) {
		std::stringstream ss;
		ss << mGame << std::endl;
		mDebugger.log(ss.str().c_str());
	}

	mDebugger.flush();
	mLastPulse = GetTickCount64();
	return !abort;
}
