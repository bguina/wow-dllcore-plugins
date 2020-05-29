#include "pch.h"

#include "../../framework.h"

#include "IWowBot.h"
#include "WowPlugin.h"

#include "../../injectable/wow/observer/ActivePlayerPositionObserver.h"

#include "../../userspace/ben/BenBots.h"
#include "../../userspace/max/MaxBot.h"
#include "../../NetworkParsing.h"

std::unique_ptr<IWowBot> getBotById(WowGame& game, long botId) {
	switch (botId) {
	case 0:
		return std::make_unique<WowMaxBot>(game);
	case 99:
		return std::make_unique<BenSimpleBot>(game);
	default:
		return std::make_unique<BenTravelBot>(game);
	}
}

WowPlugin::WowPlugin(long botId) :
	mBotPause(true),
	mGame(GetCurrentProcessId(), (const uint8_t*)GetModuleHandleA(0)),
	mDbg("WowPlugin"),
	mBot(getBotById(mGame, botId))
{
	mDbg << FileLogger::info << "Started with: " << mBot->getTag() << FileLogger::normal << std::endl;
}

WowPlugin::~WowPlugin()
{
}

void WowPlugin::onD3dRender() {
	mGame.update();
	if (!mBotPause) {
		mBot->onEvaluate();
	}
	mDbg.flush();
}

ServerWowMessage _buildMessage(Client* mClient, const std::string& messageId) {
	ServerWowMessage result(mClient);
	auto messenger = mClient->getMessageManager();

	result.cl = mClient;
	result.type = messenger.getMessageType(messageId);
	result.eject = false;
	switch (result.type) {
	case MessageType::SUBSCRIBE_DLL_UPDATES:
	case MessageType::UNSUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient->getMessageManager().getSubcribeObject(messageId);
		result.subscriptions = new std::vector<std::string>(toSubscribe.begin(), toSubscribe.end());
		break;
	}
	case MessageType::POST_DLL_DATA_3DPATH: {
		std::list<std::string> rawWaypoints = mClient->getMessageManager().getWaypointsObject(messageId);
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


bool WowPlugin::handleClient(Client& mClient) {
	std::list<std::string> messages = mClient.getMessageAvailable();

	for (std::list<std::string>::iterator msgIte = messages.begin(); msgIte != messages.end(); msgIte++) {
		const std::string& msgIdentifier(*msgIte);
		ServerWowMessage msg(_buildMessage(&mClient, msgIdentifier));

		mDbg << FileLogger::verbose << "serverMessage: " << (int)msg.type << FileLogger::normal << std::endl;

		switch (msg.type) {
		case MessageType::RESUME:
			mGame.update();
			mBotPause = false;
			mBot->onResume();
			break;

		case MessageType::PAUSE:
			mBotPause = true;
			mBot->onPause();
			break;

		case MessageType::SUBSCRIBE_DLL_UPDATES:
			//mBot->startSubscription();
			for (std::vector<std::string>::const_iterator it = msg.subscriptions->begin(); it != msg.subscriptions->end(); it++) {
				if (*it == "position") {
					mGame.addObserver("position", std::make_shared<ActivePlayerPositionObserver>(*msg.cl, 10.0f));
				}
			}
			break;

		case MessageType::UNSUBSCRIBE_DLL_UPDATES:
			//mBot->stopSubscription();
			for (std::vector<std::string>::const_iterator it = msg.subscriptions->begin(); it != msg.subscriptions->end(); it++) {
				if (*it == "position") {
					mGame.removeObserver("position");
				}
			}
			break;

		default:
			// let the bot handle it
			if (mBot->handleWowMessage(msg)) {
				mDbg << FileLogger::info << "messengable handled message " << (int)msg.type << FileLogger::normal << std::endl;
				break;
			}
			else {
				mDbg << FileLogger::err << "messengable could not handle unknown message " << (int)msg.type << FileLogger::normal << std::endl;
			}
		}

		if (nullptr != msg.subscriptions) {
			delete msg.subscriptions;
		}

		if (nullptr != msg.waypoints) {
			delete msg.waypoints;
		}

		if (msg.eject) {
			mDbg << FileLogger::err << "got ejection order " << (int)msg.type << FileLogger::normal << std::endl;
			return false;
		}
	}

	return true;
}
