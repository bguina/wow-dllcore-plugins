#include "pch.h"

#include <thread>
#include <sstream>

#include "InjectedClient.h"
#include "Client.h"
#include "../NetworkParsing.h"

/* Observers */
// Observe wow player positions

/* Userspace */

// business
#include "../injectable/wow/observer/ActivePlayerPositionObserver.h"
#include "../injectable/wow/WowPlugin.h"


/* Features */

// provide a vector<WowVector3f> to an instance of IWaypointsConsumerPlugin
#include "../injectable/wow/game/WowVector3f.h"

#include "plugin/IMessageablePlugin.h"
#include "ClientMessage.h"

InjectedClient::InjectedClient() :
	mDebugger("InjectedClient"),
	mBootTime(GetTickCount64()), 
	mLastPulse(0),
	mClient(std::make_unique<Client>())
{
	auto pid = (long)GetCurrentProcessId();

	if (mClient->joinServer()) {
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(pid));
	}

	mPlugins.push_back(std::make_shared<WowPlugin>(pid, GetModuleHandleA(0)));
}

InjectedClient::~InjectedClient() {
	mClient->disconnect();
	mDebugger << FileLogger::warn << "~InjectedClient" << FileLogger::normal << std::endl;
}

uint64_t InjectedClient::getBootTime() const { return mBootTime; }
uint64_t InjectedClient::getLastPulse() const { return mLastPulse; }

const void* InjectedClient::id() const {
	return this;
}

bool InjectedClient::throttle() const {
	return mLastPulse + 120 > GetTickCount64();
}

bool InjectedClient::run() {
	if (throttle()) return true;
	if (!mClient->isConnected() || !_dispatchMessages()) return false;


	for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
		(*it)->onD3dRender();
	}
	mLastPulse = GetTickCount64();
	mDebugger.flush();
	return true;
}

bool InjectedClient::_dispatchMessages() {
	std::list<std::string> messages = mClient->getMessageAvailable();
	bool eject = false;

	for (std::list<std::string>::iterator msgIte = messages.begin(); !eject && msgIte != messages.end(); msgIte++) {
		const std::string& msgIdentifier(*msgIte);
		ClientMessage msg(_buildMessage(msgIdentifier));

		for (auto it = mPlugins.begin(); it != mPlugins.end(); ++it) {
			auto messengable = dynamic_cast<IMessageablePlugin*>(it->get());


			if (nullptr != messengable) {
				if (messengable->handleServerMessage(msg)) {
					mDebugger << FileLogger::info << "messengable handled message " << (int)msg.type << FileLogger::normal << std::endl;
					break;
				}
				else {
					mDebugger << FileLogger::err << "messengable could not handle unknown message " << (int)msg.type << FileLogger::normal << std::endl;

					// treat as error and eject, since only a single pluggin is instanciated for now
					eject = true;
				}
			}
			else {
				mDebugger << FileLogger::err << "Running instance cannot be messenged; is not IServerPlugin" << FileLogger::normal << std::endl;
			}
		}

		if (msg.eject) {
			mDebugger << FileLogger::err << "got ejection order " << (int)msg.type << FileLogger::normal << std::endl;
			eject = true;
		}
	}

	return !eject;
}

ClientMessage InjectedClient::_buildMessage(const std::string& messageId) {
	auto messenger = mClient->getMessageManager();
	ClientMessage result;

	result.type = messenger.getMessageType(messageId);
	result.eject = false;
	switch (result.type) {
	case MessageType::SUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(messageId);

		bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());

		if (found) {

			//mGame.addObserver("position", std::make_shared<ActivePlayerPositionObserver>(*mClient, 10.0f));
		}

		break;
	}
	case MessageType::UNSUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(messageId);
		bool found = (std::find(toSubscribe.begin(), toSubscribe.end(), "position") != toSubscribe.end());
		if (found) {
			//mGame.removeObserver("position");
		}

		break;
	}
	case MessageType::POST_DLL_DATA_3DPATH: {
		std::list<std::string> rawWaypoints = mClient->getMessageManager().getWaypointsObject(messageId);

		mDebugger << "" << FileLogger::info << "WAYPOINTS" << FileLogger::normal << std::endl;
		std::vector<WowVector3f>* waypoints = new std::vector<WowVector3f>();

		for (std::list<std::string>::iterator it = rawWaypoints.begin(); it != rawWaypoints.end(); it++) {
			std::vector<std::string> rawWowVector3f;

			if (splitByDelimiter(*it, ",", rawWowVector3f) == 3) {
				waypoints->push_back(WowVector3f({ std::stof(rawWowVector3f[0]), std::stof(rawWowVector3f[1]), std::stof(rawWowVector3f[2]) }));
			}
			else throw "Bad split!";
		}

		result.waypoints = waypoints;
		break;
	}
	default:
		break;
	}

	return result;
}
