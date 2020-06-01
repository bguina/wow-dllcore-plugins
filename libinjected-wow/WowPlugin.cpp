
#include "framework.h"

#include "NetworkParsing.h"

#include "observer/ActivePlayerPositionObserver.h"
#include "WowPlugin.h"
#include "ServerWowMessage.h"

WowPlugin::WowPlugin() :
	mBotPause(true),
	mGame(std::make_shared<WowGame>(GetCurrentProcessId(), (const uint8_t*)GetModuleHandleA(0))),
	mDbg("WowPlugin"),
	mBot(nullptr),
	mClient(new Client())
{
	FileLogger dbg(mDbg, "WowPlugin()");

	if (mClient->joinServer()) {
		dbg << FileLogger::info << "connected to server" << FileLogger::normal << std::endl;
		mClient->sendMessage(mClient->getMessageManager().builRequestdDLLInjectedMessage(GetCurrentProcessId()));
	}
	else {
		dbg << FileLogger::warn << "failed to join the server" << FileLogger::normal << std::endl;
	}
}

WowPlugin::~WowPlugin() {
	FileLogger dbg(mDbg, "~WowPlugin()");
	mClient->disconnect();
	dbg << FileLogger::info << "call" << FileLogger::normal << std::endl;
}

std::string WowPlugin::getTag() const {
	return mDbg.getTag();
}

void  WowPlugin::attachBot(IWowBot* bot) {
	bot->attach(mGame);
	mBot.reset(bot);
}

bool WowPlugin::onD3dRender() {
	FileLogger dbg(mDbg, "onD3dRender");

	dbg << FileLogger::debug << "call" << FileLogger::normal << std::endl;
	if (!mClient->isConnected() || !_readServerMessages()) {
		dbg << FileLogger::warn << "lost connection with server" << FileLogger::normal << std::endl;
		return false;
	}
	else {
		dbg << FileLogger::debug << "server up-to-date" << FileLogger::normal << std::endl;
	}

	dbg << FileLogger::debug << "updating game" << FileLogger::normal << std::endl;
	mGame->update();
	if (mBot != nullptr) {
		if (!mBotPause)
			mBot->onEvaluate();
	}
	else {
		dbg << FileLogger::warn << "no bot is set" << FileLogger::normal << std::endl;
	}

	dbg << FileLogger::debug << "success" << FileLogger::normal << std::endl;

	return true;
}

ServerWowMessage _buildMessage(Client& mClient, const std::string& messageId) {
	ServerWowMessage result(&mClient);
	auto messenger = mClient.getMessageManager();

	result.cl = &mClient;
	result.type = messenger.getMessageType(messageId);
	result.eject = false;
	switch (result.type) {
	case MessageType::SUBSCRIBE_DLL_UPDATES:
	case MessageType::UNSUBSCRIBE_DLL_UPDATES: {
		std::list<std::string> toSubscribe = mClient.getMessageManager().getSubcribeObject(messageId);
		result.subscriptions = new std::vector<std::string>(toSubscribe.begin(), toSubscribe.end());
		break;
	}
	case MessageType::POST_DLL_DATA_3DPATH: {
		std::list<std::string> rawWaypoints = mClient.getMessageManager().getWaypointsObject(messageId);
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


bool WowPlugin::_readServerMessages() {
	FileLogger dbg(mDbg, "_readServerMessages");
	std::list<std::string> messages = mClient->getMessageAvailable();
	bool eject = false;

	for (std::list<std::string>::iterator msgIte = messages.begin(); !eject && msgIte != messages.end(); msgIte++) {
		const std::string& msgIdentifier(*msgIte);
		ServerWowMessage msg(_buildMessage(*mClient, msgIdentifier));

		dbg << FileLogger::verbose << "serverMessage: " << (int)msg.type << FileLogger::normal << std::endl;

		switch (msg.type) {
		case MessageType::RESUME:
			mBotPause = false;
			mBot->onResume();
			break;

		case MessageType::POST_SERVER_EJECTION:
			msg.eject = true;
		case MessageType::PAUSE:
			mBotPause = true;
			mBot->onPause();
			break;

		case MessageType::SUBSCRIBE_DLL_UPDATES:
			//mBot->startSubscription();
			for (std::vector<std::string>::const_iterator it = msg.subscriptions->begin(); it != msg.subscriptions->end(); it++) {
				if (*it == "position") {
					mGame->addObserver("position", std::make_shared<ActivePlayerPositionObserver>(*msg.cl, 10.0f));
				}
			}
			break;

		case MessageType::UNSUBSCRIBE_DLL_UPDATES:
			//mBot->stopSubscription();
			for (std::vector<std::string>::const_iterator it = msg.subscriptions->begin(); it != msg.subscriptions->end(); it++) {
				if (*it == "position") {
					mGame->removeObserver("position");
				}
			}
			break;

		default:
			// let the bot handle it
			if (mBot->handleWowMessage(msg)) {
				dbg << FileLogger::info << "messengable handled message " << (int)msg.type << FileLogger::normal << std::endl;
				break;
			}
			else {
				dbg << FileLogger::err << "messengable could not handle unknown message " << (int)msg.type << FileLogger::normal << std::endl;
			}
		}

		if (nullptr != msg.subscriptions) {
			delete msg.subscriptions;
		}

		if (nullptr != msg.waypoints) {
			delete msg.waypoints;
		}

		if (msg.eject) {
			eject = true;
			dbg << FileLogger::err << "got ejection order " << (int)msg.type << FileLogger::normal << std::endl;
			break;
		}
	}
	dbg << FileLogger::verbose << "read of " << messages.size() << FileLogger::normal << std::endl;
	return !eject;
}
