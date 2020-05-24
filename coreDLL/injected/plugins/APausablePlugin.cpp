#include "pch.h"

#include "APausablePlugin.h"

#include "PluginServerMessage.h"

APausablePlugin::APausablePlugin(const std::string& tag) :
	IPausablePlugin(),
	mPaused(true),
	mDbg(tag)
{
}

APausablePlugin::~APausablePlugin()
{
}

const std::string& APausablePlugin::getTag() const {
	return mDbg.getTag();
}

bool APausablePlugin::handleServerMessage(const PluginServerMessage& serverMessage) {
	mDbg << FileLogger::info << "APausablePlugin: handleServerMessage" << (int)serverMessage.type << FileLogger::normal << std::endl;

	switch (serverMessage.type) {
	case MessageType::RESUME_PLUGIN:
		mDbg << FileLogger::info << "APausablePlugin: RESUME_PLUGIN" << FileLogger::normal << std::endl;
		pause(false);
		break;
	case MessageType::PAUSE_PLUGIN:
		mDbg << FileLogger::info << "APausablePlugin: PAUSE_PLUGIN" << FileLogger::normal << std::endl;
		pause(true);
		break;
	default: return false;
	}
	return true;
}

bool APausablePlugin::pause(bool paused) {
	if (paused == mPaused) {
		// failed to take effect, already in desired state
		mDbg << FileLogger::err << "APausablePlugin: failed to toggle pause, already in paused=" << paused << " state" << FileLogger::normal << std::endl;
		return false;
	}

	mPaused = paused;
	mDbg << FileLogger::warn << "APausablePlugin: toggled pause" << FileLogger::normal << std::endl;
	return true;
}

bool APausablePlugin::isPaused() const {
	return mPaused;
}
