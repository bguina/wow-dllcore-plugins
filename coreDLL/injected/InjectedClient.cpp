#include "pch.h"

#include <thread>
#include <sstream>

#include "InjectedClient.h"
#include "Client.h"
#include "../NetworkParsing.h"

/* Observers */
// Observe wow player positions
#include "observer/wow/ActivePlayerPositionObserver.h"

/* Instanciators */
// Wow bots
#include "plugins/ben/BenFightBot.h"
#include "plugins/ben/BenTravelBot.h"
#include "plugins/max/MaxBot.h"

/* Features */

// provide a vector<vector3f> to an instance of IWaypointsConsumerPlugin
#include "../pathfinder/Vector3f.h"

#include "plugins/PluginServerMessage.h"

InjectedClient::InjectedClient() :
	mDebugger("InjectedClient"),
	mBootTime(GetTickCount64()), mLastPulse(0),
	mClient(std::make_unique<Client>()),
	mGame((const uint8_t*)GetModuleHandleA(0)),
	mPlugin(nullptr)
{
	if (mClient->joinServer()) {
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(getGame().getPid()));
	}

	int botSelection = 43;

	switch (botSelection) {
	case 0:
		mDebugger << FileLogger::info << "creating WowMaxBot" << FileLogger::normal << std::endl;
		mPlugin = std::make_unique<WowMaxBot>(mGame);
		break;
	case 42:
		mDebugger << FileLogger::info << "creating BenTravelBot" << FileLogger::normal << std::endl;
		mPlugin = std::make_unique<BenTravelBot>(mGame);
		break;
	default:
		mDebugger << FileLogger::info << "creating BenFightBot" << FileLogger::normal << std::endl;
		mPlugin = std::make_unique<BenFightBot>(mGame);
		break;
	}
}

InjectedClient::~InjectedClient() {
	mClient->disconnect();
	mDebugger << FileLogger::warn << "~InjectedClient" << FileLogger::normal << std::endl;
}

uint64_t InjectedClient::getBootTime() const { return mBootTime; }
uint64_t InjectedClient::getLastPulse() const { return mLastPulse; }
const WowGame& InjectedClient::getGame() const { return mGame; }

const void* InjectedClient::id() const {
	return this;
}

bool InjectedClient::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool InjectedClient::run() {
	if (throttle()) return true;
	if (!mClient->isConnected() || !_dispatchMessages()) return false;

	mGame.update();
	mPlugin->onD3dRender();
	mLastPulse = GetTickCount64();
	mDebugger.flush();
	return true;
}

bool InjectedClient::_dispatchMessages() {
	std::list<std::string> messages = mClient->getMessageAvailable();

	for (std::list<std::string>::iterator msgIte = messages.begin(); msgIte != messages.end(); msgIte++) {
		const std::string& msgIdentifier(*msgIte);
		PluginServerMessage msg(_buildMessage(msgIdentifier));

		if (msg.eject) {
			mDebugger << FileLogger::err << "got ejection order " << (int)msg.type << FileLogger::normal << std::endl;
			return false;
		}

		auto messengable = dynamic_cast<IServerPlugin*>(mPlugin.get());
		if (nullptr != messengable) {
			if (!messengable->handleServerMessage(msg)) {
				mDebugger << FileLogger::err << "messengable could not handle message " << (int)msg.type << FileLogger::normal << std::endl;
				return false;
			}
			else {
				mDebugger << FileLogger::info << "messengable handled message " << (int)msg.type << FileLogger::normal << std::endl;
			}
		}
		else {
			mDebugger << FileLogger::err << "Running instance cannot be messenged; is not IServerPlugin" << FileLogger::normal << std::endl;
		}
	}
	return true;
}

PluginServerMessage InjectedClient::_buildMessage(const std::string& messageId) {
	auto messenger = mClient->getMessageManager();
	PluginServerMessage result;

	result.type = messenger.getMessageType(messageId);
	result.eject = false;
	switch (result.type) {
	case MessageType::SUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(messageId);

		bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

		if (found) {
			mGame.addObserver("position", std::make_shared<ActivePlayerPositionObserver>(*mClient, 10.0f));
		}

		break;
	}
	case MessageType::UNSUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(messageId);
		bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
		if (found) {
			mGame.removeObserver("position");
		}

		break;
	}
	case MessageType::POST_DLL_DATA_3DPATH: {
		std::list<std::string> rawWaypoints = mClient->getMessageManager().getWaypointsObject(messageId);

		mDebugger << "" << FileLogger::info << "WAYPOINTS" << FileLogger::normal << std::endl;
		std::vector<Vector3f>* waypoints = new std::vector<Vector3f>();

		for (std::list<std::string>::iterator it = rawWaypoints.begin(); it != rawWaypoints.end(); it++) {
			std::vector<std::string> rawVector3f;

			if (splitByDelimiter(*it, ",", rawVector3f) == 3) {
				waypoints->push_back(Vector3f({ std::stof(rawVector3f[0]), std::stof(rawVector3f[1]), std::stof(rawVector3f[2]) }));
			}
			else throw "Bad split!";
		}

		result.waypoints = waypoints;
		/*auto waypointLoader = dynamic_cast<IWaypointsConsumerPlugin*>(mPlugin.get());
		if (nullptr != waypointLoader) {
			mDebugger << FileLogger::err << "Loading " << waypoints->size() << " waypoints" << FileLogger::normal << std::endl;
			waypointLoader->loadWaypoints(*result.data.waypoints);
		}
		else {
			mDebugger << "" << FileLogger::err << "Running instance does not implement IPathWaypointsConsumer" << FileLogger::normal << std::endl;
		}*/
		break;
	}
	case MessageType::RESUME_PLUGIN: {
		mDebugger << "" << FileLogger::warn << "START_BOT" << FileLogger::normal << std::endl;
		/*
		auto pausable = dynamic_cast<IPausablePlugin*>(mPlugin.get());
		if (nullptr != pausable) {
			pausable->pause(false);
		}
		else {
			mDebugger << "" << FileLogger::err << "Running instance cannot be started; is not pausable" << FileLogger::normal << std::endl;
		}*/
		break;
	}
	case MessageType::PAUSE_PLUGIN: {
		mDebugger << FileLogger::warn << "STOP_BOT" << FileLogger::normal << std::endl;
		/*auto pausable = dynamic_cast<IPausablePlugin*>(mPlugin.get());
		if (nullptr != pausable) {
			pausable->pause(false);
		}
		else {
			mDebugger << FileLogger::err << "Running instance cannot be paused; is not pausable" << FileLogger::normal << std::endl;
		}*/
		break;
	}
	case MessageType::POST_SERVER_EJECTION: {
		result.eject = true;
		break;
	}
	default:
		mDebugger << FileLogger::err << "UNKNOWN SERVER MESSAGE " << (int)result.type << FileLogger::normal << std::endl;
		break;
	}

	return result;
}
