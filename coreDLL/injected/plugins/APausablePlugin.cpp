#include "pch.h"

#include "APausablePlugin.h"

#include "PluginServerMessage.h"

APausablePlugin::APausablePlugin() :
	mPaused(true)
{
}

APausablePlugin::~APausablePlugin()
{
}

bool APausablePlugin::handleServerMessage(const PluginServerMessage& serverMessage) {
	switch (serverMessage.type) {
	case MessageType::START_BOT:
	case MessageType::STOP_BOT:

		pause(MessageType::STOP_BOT == serverMessage.type);
		return true;

	default: break;
	}
	return false;
}

bool APausablePlugin::pause(bool paused) {
	if (paused == mPaused) {
		// failed to take effect, already in desired state
		return false;
	}

	mPaused = paused;
	return true;
}

bool APausablePlugin::isPaused() const {
	return mPaused;
}
