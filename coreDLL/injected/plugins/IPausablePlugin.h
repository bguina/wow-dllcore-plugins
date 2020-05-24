#pragma once

#include <string>

#include "IServerPlugin.h"

class IPausablePlugin : public IServerPlugin
{
public:
	virtual ~IPausablePlugin() {};

	virtual const std::string& getTag() const = 0;

	virtual bool pause(bool paused) = 0;
	virtual bool isPaused() const = 0;
};
