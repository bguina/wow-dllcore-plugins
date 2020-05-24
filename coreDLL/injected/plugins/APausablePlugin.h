#pragma once

#include "IPausablePlugin.h"

#include "../../logger/FileLogger.h"

class APausablePlugin : public IPausablePlugin
{
public:
	APausablePlugin(const std::string& tag);
	virtual ~APausablePlugin();

	virtual const std::string& getTag() const override;

	virtual bool pause(bool paused);
	virtual bool isPaused() const;

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) override;

protected:
	FileLogger mDbg;
	bool mPaused;
};
