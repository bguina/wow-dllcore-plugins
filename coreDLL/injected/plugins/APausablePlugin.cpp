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
	case MessageType::RESUME_PLUGIN:
	case MessageType::PAUSE_PLUGIN:
		pause(MessageType::PAUSE_PLUGIN == serverMessage.type);
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
