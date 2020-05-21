#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "observers/ActivePlayerPositionObserver.h"

#include "NetworkParsing.h"

bool readMessageAvailable(ServerSDK& server, WowGame& game, WowBot& bot) {
	std::list<std::string> messages = server.getMessageAvailable();
	for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		switch (server.getMessageManager().getMessageType((*it)))
		{
		case MessageType::START_SUBSCRIBE: {
			std::list<std::string> toSubscribe = server.getMessageManager().getSubcribeObject(*it);

			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

			if (found) {
				game.addObserver("position", new ActivePlayerPositionObserver(server, 10));
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
			std::list<std::string> rawWaypoints = server.getMessageManager().getWaypointsObject(*it);
			std::vector<Vector3f> waypoints;

			for (std::list<std::string>::iterator it = rawWaypoints.begin(); it != rawWaypoints.end(); it++) {
				std::vector<std::string> rawVector3f;

				if (splitByDelimiter(*it, ",", rawVector3f) == 3) {
					waypoints.push_back(Vector3f(std::stof(rawVector3f[0]), std::stof(rawVector3f[1]), std::stof(rawVector3f[2])));
				}
				else throw "Bad split!";
			}
			bot.loadLinearWaypoints(waypoints);
			break;
		}
		case MessageType::START_BOT: {
			bot.setBotStarted(true);
			break;
		}
		case MessageType::STOP_BOT: {
			bot.setBotStarted(false);
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
}

ULONG64 Sandbox::getBootTime() const { return mBootTime; }
ULONG64 Sandbox::getLastPulse() const { return mLastPulse; }
WowGame Sandbox::getGame() const { return mGame; }

bool Sandbox::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool Sandbox::run(ServerSDK& server) {
	if (throttle()) return true;

	bool abort = !readMessageAvailable(server, mGame, mBot);

	mGame.update();
	mBot.run();
	mDebugger.flush();
	mLastPulse = GetTickCount64();
	return !abort;
}
