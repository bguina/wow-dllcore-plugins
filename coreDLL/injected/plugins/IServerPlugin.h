#pragma once

#include "IPlugin.h"

class PluginServerMessage;

class IServerPlugin : public IPlugin
{
public:
	virtual ~IServerPlugin() {};

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) = 0;
};
