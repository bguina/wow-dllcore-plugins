#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "pathfinder/Vector3f.h"
#include "injected/wow/observers/ActivePlayerPositionObserver.h"

#include "pathfinder/Vector3f.h"
#include "NetworkParsing.h"

bool Sandbox::stackServerMessages(ServerSDK& server) {
	std::list<std::string> messages = server.getMessageAvailable();

	for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		auto messageType = server.getMessageManager().getMessageType((*it));

		switch (messageType)
		{
		case MessageType::START_SUBSCRIBE: {
			std::list<std::string> toSubscribe = server.getMessageManager().getSubcribeObject(*it);

			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

			if (found) {
				mGame.addObserver("position", std::make_shared<ActivePlayerPositionObserver>(server, 10.0f));
			}

			break;
		}
		case MessageType::STOP_SUBSCRIBE: {
			std::list<std::string> toSubscribe = server.getMessageManager().getSubcribeObject(*it);
			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
			if (found) {
				mGame.removeObserver("position");
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

			mBot.loadLinearWaypoints(waypoints);
			break;
		}
		case MessageType::START_BOT: {
			mBot.pause(false);
			break;
		}
		case MessageType::STOP_BOT: {
			mBot.pause(true);
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
	mDebugger("Sandbox"),
	mBootTime(GetTickCount64()), mLastPulse(0),
	mGame((const uint8_t*)GetModuleHandleA(0)),
	mBot(mGame)
{
	// TODO connect to server from here
	// TODO clear log files?
}

Sandbox::~Sandbox() {
	// TODO disconnect from server in case we are still connected

	mDebugger.log("~Sandbox");
}

ULONG64 Sandbox::getBootTime() const { return mBootTime; }
ULONG64 Sandbox::getLastPulse() const { return mLastPulse; }
const WowGame& Sandbox::getGame() const { return mGame; }

bool Sandbox::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool Sandbox::run(ServerSDK& server) {
	if (throttle()) return true;

	std::stringstream ss;

	bool abort = !stackServerMessages(server);
	ss << "abort? " << abort << std::endl;

	mGame.update();
	mBot.run();
	ss << FileDebugger::info << "bot.run" << std::endl;

	mLastPulse = GetTickCount64();

	mDebugger << ss.str();
	mDebugger.flush();
	return !abort;
}
