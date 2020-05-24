#include "pch.h"

#include "APausablePlugin.h"

#include "PluginServerMessage.h"

APausablePlugin::APausablePlugin(const std::string& tag) :
	mPaused(true),
	mDbg(tag)
{
}

APausablePlugin::~APausablePlugin()
{
}

bool APausablePlugin::handleServerMessage(const PluginServerMessage& serverMessage) {
	mDbg << FileLogger::info << "APausablePlugin::handleServerMessage" << (int)serverMessage.type << FileLogger::normal << std::endl;

	switch (serverMessage.type) {
	case MessageType::RESUME_PLUGIN:
		pause(false);
		return true;

	case MessageType::PAUSE_PLUGIN:
		pause(true);
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
