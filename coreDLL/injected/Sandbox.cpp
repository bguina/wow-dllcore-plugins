#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "Client.h"
#include "../pathfinder/Vector3f.h"
#include "wow/observers/ActivePlayerPositionObserver.h"
#include "../NetworkParsing.h"

bool Sandbox::stackServerMessages() {
	std::list<std::string> messages = mClient->getMessageAvailable();

	for (std::list<std::string>::iterator msgIte = messages.begin(); msgIte != messages.end(); msgIte++)
	{
		auto messageType = mClient->getMessageManager().getMessageType((*msgIte));

		switch (messageType)
		{
		case MessageType::START_SUBSCRIBE: {
			std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(*msgIte);

			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

			if (found) {
				mGame.addObserver("position", std::make_shared<ActivePlayerPositionObserver>(*mClient, 10.0f));
			}

			break;
		}
		case MessageType::STOP_SUBSCRIBE: {
			std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(*msgIte);
			bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
			if (found) {
				mGame.removeObserver("position");
			}

			break;
		}
		case MessageType::WAYPOINTS: {
			std::list<std::string> rawWaypoints = mClient->getMessageManager().getWaypointsObject(*msgIte);
			std::vector<Vector3f> waypoints;

			for (std::list<std::string>::iterator it = rawWaypoints.begin(); it != rawWaypoints.end(); it++) {
				std::vector<std::string> rawVector3f;

				if (splitByDelimiter(*it, ",", rawVector3f) == 3) {
					waypoints.push_back(Vector3f({ std::stof(rawVector3f[0]), std::stof(rawVector3f[1]), std::stof(rawVector3f[2]) }));
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
	mClient(std::make_unique<Client>()),
	mGame((const uint8_t*)GetModuleHandleA(0)),
	mBot(mGame)
{
	if (mClient->joinServer()) {
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(getGame().getPid()));
	}
}

Sandbox::~Sandbox() {
	mClient->disconnect();
	mDebugger.log("~Sandbox");
}

uint64_t Sandbox::getBootTime() const { return mBootTime; }
uint64_t Sandbox::getLastPulse() const { return mLastPulse; }
const WowGame& Sandbox::getGame() const { return mGame; }

const void* Sandbox::id() const {
	return this;
}

bool Sandbox::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool Sandbox::run() {
	if (throttle()) return true;
	if (!mClient->isConnected() || !stackServerMessages()) return false;

	std::stringstream ss;
	ss << "abort? " << abort << std::endl;

	mGame.update();
	mBot.run();
	ss << FileDebugger::info << "bot.run" << std::endl;

	mLastPulse = GetTickCount64();

	mDebugger << ss.str();
	mDebugger.flush();
	return true;
}
