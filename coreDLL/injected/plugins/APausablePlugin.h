#pragma once

#include "IPausablePlugin.h"

class APausablePlugin : public IPausablePlugin
{
public:
	APausablePlugin();
	virtual ~APausablePlugin();

	virtual bool pause(bool paused);
	virtual bool isPaused() const;

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) override;

protected:
	bool mPaused;
};
