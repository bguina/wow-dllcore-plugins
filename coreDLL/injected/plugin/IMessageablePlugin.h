#pragma once

#include "IPlugin.h"

class ClientMessage;

class IMessageablePlugin : public IPlugin
{
public:
	virtual ~IMessageablePlugin() {};

	virtual bool handleServerMessage(ClientMessage& serverMessage) = 0;
};
