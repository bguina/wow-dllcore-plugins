#pragma once

#include "IServerPlugin.h"

class IPausablePlugin : public IServerPlugin
{
public:
	virtual ~IPausablePlugin() {};
	virtual bool pause(bool paused) = 0;
	virtual bool isPaused() const = 0;
};
