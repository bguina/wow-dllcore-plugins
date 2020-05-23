#include "pch.h"

#include <thread>
#include <sstream>

#include "Sandbox.h"
#include "Client.h"
#include "../NetworkParsing.h"

/* Observers */
// Observe wow player positions
#include "wow/observers/ActivePlayerPositionObserver.h"

/* Instanciators */
// Wow bots
#include "wow/bot/ben/BenFightBot.h"
#include "wow/bot/ben/BenTravelBot.h"
#include "wow/bot/max/MaxBot.h"

/* Features */

// provide a vector<vector3f> to an instance
#include "../pathfinder/IPathWaypointsConsumer.h"

bool Sandbox::stackServerMessages() {
	std::list<std::string> messages = mClient->getMessageAvailable();

	for (std::list<std::string>::iterator msgIte = messages.begin(); msgIte != messages.end(); msgIte++)
	{
		auto messageType = mClient->getMessageManager().getMessageType((*msgIte));

		switch (messageType) {
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
			mDebugger << FileDebugger::info << "WAYPOINTS" << FileDebugger::normal << std::endl;

			for (std::list<std::string>::iterator it = rawWaypoints.begin(); it != rawWaypoints.end(); it++) {
				std::vector<std::string> rawVector3f;

				if (splitByDelimiter(*it, ",", rawVector3f) == 3) {
					waypoints.push_back(Vector3f({ std::stof(rawVector3f[0]), std::stof(rawVector3f[1]), std::stof(rawVector3f[2]) }));
				}
				else throw "Bad split!";
			}

			auto waypointLoader = dynamic_cast<IPathWaypointsConsumer*>(mBot.get());
			if (nullptr != waypointLoader) {
				mDebugger << FileDebugger::err << "Loading " << waypoints.size() << " waypoints" << FileDebugger::normal << std::endl;
				waypointLoader->loadPathWaypoints(waypoints);
			}
			else {
				mDebugger << FileDebugger::err << "Running instance does not implement IPathWaypointsConsumer" << FileDebugger::normal << std::endl;
			}
			break;
		}
		case MessageType::START_BOT: {
			mDebugger << FileDebugger::warn << "START_BOT" << FileDebugger::normal << std::endl;
			mBot->pause(false);
			break;
		}
		case MessageType::STOP_BOT: {
			mDebugger << FileDebugger::warn << "STOP_BOT" << FileDebugger::normal << std::endl;
			mBot->pause(true);
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
	mBot(nullptr)
{
	if (mClient->joinServer()) {
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(getGame().getPid()));
	}

	int botSelection = 1;

	switch (botSelection) {
	case 0:
		mDebugger << FileDebugger::info << "creating WowMaxBot" << FileDebugger::normal << std::endl;
		mBot = std::make_unique<WowMaxBot>(mGame);
		break;
	case 42:
		mDebugger << FileDebugger::info << "creating BenTravelBot" << FileDebugger::normal << std::endl;
		mBot = std::make_unique<BenTravelBot>(mGame);
		break;
	default:
		mDebugger << FileDebugger::info << "creating BenFightBot" << FileDebugger::normal << std::endl;
		mBot = std::make_unique<BenFightBot>(mGame);
		break;
	}
}

Sandbox::~Sandbox() {
	mClient->disconnect();
	mDebugger << FileDebugger::warn << "~Sandbox" << FileDebugger::normal << std::endl;
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

	mGame.update();
	mBot->run();
	mLastPulse = GetTickCount64();
	mDebugger.flush();
	return true;
}
